/*
 *  PKCS11-LOGGER - PKCS#11 logging proxy module
 *  Copyright (c) 2011-2015 JWC s.r.o. <http://www.jwc.sk>
 *  Author: Jaroslav Imrich <jimrich@jimrich.sk>
 *
 *  Licensing for open source projects:
 *  PKCS11-LOGGER is available under the terms of the GNU Affero General 
 *  Public License version 3 as published by the Free Software Foundation.
 *  Please see <http://www.gnu.org/licenses/agpl-3.0.html> for more details.
 *
 *  Licensing for other types of projects:
 *  PKCS11-LOGGER is available under the terms of flexible commercial license.
 *  Please contact JWC s.r.o. at <info@pkcs11interop.net> for more details.
 */


#include "pkcs11-logger.h"


extern PKCS11_LOGGER_GLOBALS pkcs11_logger_globals;


// Loads original PKCS#11 library
int pkcs11_logger_init_orig_lib(void)
{
    CK_C_GetFunctionList GetFunctionListPointer = NULL;
    CK_RV rv = CKR_OK;

    if (NULL != pkcs11_logger_globals.orig_lib_handle)
        return PKCS11_LOGGER_RV_SUCCESS;

    // Create lock for synchronization of log file access
    if (PKCS11_LOGGER_RV_SUCCESS != pkcs11_logger_lock_create())
        return PKCS11_LOGGER_RV_ERROR;

    // Read environment variables
    if (PKCS11_LOGGER_RV_SUCCESS != pkcs11_logger_init_parse_env_vars())
        return PKCS11_LOGGER_RV_ERROR;

    pkcs11_logger_globals.env_vars_read = CK_TRUE;

    // Log informational header
    pkcs11_logger_log_separator();
    pkcs11_logger_log("%s %s", PKCS11_LOGGER_NAME, PKCS11_LOGGER_VERSION);
    pkcs11_logger_log("%s", PKCS11_LOGGER_DESCRIPTION);
    pkcs11_logger_log("Developed as a part of the Pkcs11Interop project");
    pkcs11_logger_log("Please visit www.pkcs11interop.net for more information");
    pkcs11_logger_log_separator();

    // Load PKCS#11 library
    pkcs11_logger_globals.orig_lib_handle = DLOPEN((const char *)pkcs11_logger_globals.env_var_library_path);
    if (NULL == pkcs11_logger_globals.orig_lib_handle)
    {
        pkcs11_logger_log("Unable to load %s", pkcs11_logger_globals.env_var_library_path);
        return PKCS11_LOGGER_RV_ERROR;
    }

    // Get pointer to C_GetFunctionList()
    GetFunctionListPointer = (CK_C_GetFunctionList) DLSYM(pkcs11_logger_globals.orig_lib_handle, "C_GetFunctionList");
    if (NULL == GetFunctionListPointer)
    {
        pkcs11_logger_log("Unable to find C_GetFunctionList() in %s", pkcs11_logger_globals.env_var_library_path);
        CALL_N_CLEAR(DLCLOSE, pkcs11_logger_globals.orig_lib_handle);
        return PKCS11_LOGGER_RV_ERROR;
    }

    // Get pointers to all PKCS#11 functions
    rv = GetFunctionListPointer(&(pkcs11_logger_globals.orig_lib_functions));
    if (CKR_OK != rv)
    {
        pkcs11_logger_log("Unable to call C_GetFunctionList() from %s", pkcs11_logger_globals.env_var_library_path);
        CALL_N_CLEAR(DLCLOSE, pkcs11_logger_globals.orig_lib_handle);
        return PKCS11_LOGGER_RV_ERROR;
    }

    // Lets present version of orig library as ours - that's what proxies do :)
    pkcs11_logger_globals.logger_functions.version.major = pkcs11_logger_globals.orig_lib_functions->version.major;
    pkcs11_logger_globals.logger_functions.version.minor = pkcs11_logger_globals.orig_lib_functions->version.minor;
    
    // Everything is set up
    pkcs11_logger_log("Successfuly loaded %s", pkcs11_logger_globals.env_var_library_path);
    pkcs11_logger_log("Memory contents are dumped without endianness conversion");

    return PKCS11_LOGGER_RV_SUCCESS;
}


// Parses environment variables
int pkcs11_logger_init_parse_env_vars(void)
{
    int rv = PKCS11_LOGGER_RV_ERROR;

    // Read PKCS11_LOGGER_LIBRARY_PATH environment variable
    pkcs11_logger_globals.env_var_library_path = pkcs11_logger_init_read_env_var(PKCS11_LOGGER_LIBRARY_PATH);
    if (NULL == pkcs11_logger_globals.env_var_library_path)
    {
        pkcs11_logger_log("Environment variable %s is not defined", PKCS11_LOGGER_LIBRARY_PATH);
        goto err;
    }

    if (('"' == pkcs11_logger_globals.env_var_library_path[0]) || ('\'' == pkcs11_logger_globals.env_var_library_path[0]))
    {
        pkcs11_logger_log("Value of %s environment variable needs to be provided without enclosing quotes", PKCS11_LOGGER_LIBRARY_PATH);
        goto err;
    }

    // Read PKCS11_LOGGER_LOG_FILE_PATH environment variable
    pkcs11_logger_globals.env_var_log_file_path = pkcs11_logger_init_read_env_var(PKCS11_LOGGER_LOG_FILE_PATH);
    if (NULL != pkcs11_logger_globals.env_var_log_file_path)
    {
        if (('"' == pkcs11_logger_globals.env_var_log_file_path[0]) || ('\'' == pkcs11_logger_globals.env_var_log_file_path[0]))
        {
            pkcs11_logger_log("Value of %s environment variable needs to be provided without enclosing quotes", PKCS11_LOGGER_LOG_FILE_PATH);
            goto err;
        }
    }

    // Read PKCS11_LOGGER_FLAGS environment variable
    pkcs11_logger_globals.env_var_flags = pkcs11_logger_init_read_env_var(PKCS11_LOGGER_FLAGS);
    if (NULL != pkcs11_logger_globals.env_var_flags)
    {
        if (PKCS11_LOGGER_RV_SUCCESS != pkcs11_logger_utils_str_to_long((const char *)pkcs11_logger_globals.env_var_flags, &(pkcs11_logger_globals.flags)))
        {
            pkcs11_logger_log("Unable to read the value of %s environment variable as a number", PKCS11_LOGGER_FLAGS);
            goto err;
        }
    }

    rv = PKCS11_LOGGER_RV_SUCCESS;

err:

    if (rv == PKCS11_LOGGER_RV_ERROR)
    {
        CALL_N_CLEAR(free, pkcs11_logger_globals.env_var_library_path);
        CALL_N_CLEAR(free, pkcs11_logger_globals.env_var_log_file_path);
        CALL_N_CLEAR(free, pkcs11_logger_globals.env_var_flags);
    }

    return rv;
}


// Reads environment variable
CK_CHAR_PTR pkcs11_logger_init_read_env_var(const char *env_var_name)
{
    CK_CHAR_PTR output_value = NULL;

#ifdef _WIN32

    LPSTR env_var_value = NULL;
    DWORD env_var_value_size = 0;

    env_var_value_size = GetEnvironmentVariableA(env_var_name, env_var_value, env_var_value_size);
    if (0 == env_var_value_size)
    {
        // Note: Environment variable is not defined
        goto err;
    }

    env_var_value = (LPSTR) malloc(env_var_value_size);
    if (NULL == env_var_value)
    {
        pkcs11_logger_log("Unable to allocate memory for the value of %s environment variable", env_var_name);
        goto err;
    }

    if ((env_var_value_size - 1) != GetEnvironmentVariableA(env_var_name, env_var_value, env_var_value_size))
    {
        pkcs11_logger_log("Unable to read the value of %s environment variable", env_var_name);
        goto err;
    }

    output_value = (CK_CHAR_PTR) env_var_value;
    env_var_value = NULL;

#else

    char *env_var_value = NULL;

    env_var_value = getenv(env_var_name);
    if (NULL == env_var_value)
    {
        // Note: Environment variable is not defined
        goto err;
    }

    output_value = (CK_CHAR_PTR) strdup(env_var_value);
    if (NULL == output_value)
    {
        pkcs11_logger_log("Unable to copy the value of %s environment variable", env_var_name);
        goto err;
    }

#endif

err:

    CALL_N_CLEAR(free, env_var_value);

    return output_value;
}
