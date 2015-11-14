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


extern DLHANDLE pkcs11_logger_orig_lib;
extern CK_FUNCTION_LIST_PTR pkcs11_logger_orig_lib_functions;
extern CK_BBOOL pkcs11_logger_env_vars_read;
extern CK_CHAR_PTR pkcs11_logger_library_path;
extern CK_CHAR_PTR pkcs11_logger_log_file_path;
extern CK_ULONG pkcs11_logger_flags;
extern CK_FUNCTION_LIST pkcs11_logger_functions;


// Loads original PKCS#11 library
int pkcs11_logger_init_orig_lib(void)
{
    CK_C_GetFunctionList GetFunctionListPointer = NULL;
    CK_RV rv = CKR_OK;

    if (NULL != pkcs11_logger_orig_lib)
        return PKCS11_LOGGER_RV_SUCCESS;

    // Create lock for synchronization of log file access
    if (PKCS11_LOGGER_RV_SUCCESS != pkcs11_logger_lock_create())
        return PKCS11_LOGGER_RV_ERROR;

    // Read environment variables
    if (PKCS11_LOGGER_RV_SUCCESS != pkcs11_logger_init_parse_env_vars())
        return PKCS11_LOGGER_RV_ERROR;

    pkcs11_logger_env_vars_read = CK_TRUE;

    // Log informational header
    pkcs11_logger_log_separator();
    pkcs11_logger_log("%s %s", PKCS11_LOGGER_NAME, PKCS11_LOGGER_VERSION);
    pkcs11_logger_log("%s", PKCS11_LOGGER_DESCRIPTION);
    pkcs11_logger_log("Developed as a part of the Pkcs11Interop project");
    pkcs11_logger_log("Please visit www.pkcs11interop.net for more information");
    pkcs11_logger_log_separator();

    // Load PKCS#11 library
    pkcs11_logger_orig_lib = DLOPEN((const char *) pkcs11_logger_library_path);
    if (NULL == pkcs11_logger_orig_lib)
    {
        pkcs11_logger_log("Unable to load %s", pkcs11_logger_library_path);
        return PKCS11_LOGGER_RV_ERROR;
    }

    // Get pointer to C_GetFunctionList()
    GetFunctionListPointer = (CK_C_GetFunctionList) DLSYM(pkcs11_logger_orig_lib, "C_GetFunctionList");
    if (NULL == GetFunctionListPointer)
    {
        pkcs11_logger_log("Unable to find C_GetFunctionList() in %s", pkcs11_logger_library_path);
        CALL_N_CLEAR(DLCLOSE, pkcs11_logger_orig_lib);
        return PKCS11_LOGGER_RV_ERROR;
    }

    // Get pointers to all PKCS#11 functions
    rv = GetFunctionListPointer(&pkcs11_logger_orig_lib_functions);
    if (CKR_OK != rv)
    {
        pkcs11_logger_log("Unable to call C_GetFunctionList() from %s", pkcs11_logger_library_path);
        CALL_N_CLEAR(DLCLOSE, pkcs11_logger_orig_lib);
        return PKCS11_LOGGER_RV_ERROR;
    }

    // Lets present version of orig library as ours - that's what proxies do :)
    pkcs11_logger_functions.version.major = pkcs11_logger_orig_lib_functions->version.major;
    pkcs11_logger_functions.version.minor = pkcs11_logger_orig_lib_functions->version.minor;
    
    // Everything is set up
    pkcs11_logger_log("Successfuly loaded %s", pkcs11_logger_library_path);
    pkcs11_logger_log("Memory contents are dumped without endianness conversion");

    return PKCS11_LOGGER_RV_SUCCESS;
}


// Parses environment variables
int pkcs11_logger_init_parse_env_vars(void)
{
    int rv = PKCS11_LOGGER_RV_ERROR;
    char *env_var = NULL;

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable: 4996)
#endif

    // Read PKCS11_LOGGER_LIBRARY_PATH environment variable
    env_var = getenv(PKCS11_LOGGER_LIBRARY_PATH);
    if (NULL == env_var)
    {
        pkcs11_logger_log("Environment variable PKCS11_LOGGER_LIBRARY_PATH is not defined");
        goto err;
    }

    if (('"' == env_var[0]) || ('\'' == env_var[0]))
    {
        pkcs11_logger_log("Value of PKCS11_LOGGER_LIBRARY_PATH environment variable needs to be provided without enclosing quotes");
        goto err;
    }

    pkcs11_logger_library_path = (CK_CHAR_PTR) STRDUP(env_var);
    if (NULL == pkcs11_logger_library_path)
    {
        pkcs11_logger_log("Unable to copy the value of PKCS11_LOGGER_LIBRARY_PATH environment variable");
        goto err;
    }

    // Read PKCS11_LOGGER_LOG_FILE_PATH environment variable
    env_var = getenv(PKCS11_LOGGER_LOG_FILE_PATH);
    if (NULL != env_var)
    {
        if (('"' == env_var[0]) || ('\'' == env_var[0]))
        {
            pkcs11_logger_log("Value of PKCS11_LOGGER_LOG_FILE_PATH environment variable needs to be provided without enclosing quotes");
            goto err;
        }

        pkcs11_logger_log_file_path = (CK_CHAR_PTR) STRDUP(env_var);
        if (NULL == pkcs11_logger_log_file_path)
        {
            pkcs11_logger_log("Unable to copy the value of PKCS11_LOGGER_LOG_FILE_PATH environment variable");
            goto err;
        }
    }

    // Read PKCS11_LOGGER_FLAGS environment variable
    env_var = getenv(PKCS11_LOGGER_FLAGS);
    if (NULL != env_var)
    {
        if (PKCS11_LOGGER_RV_SUCCESS != pkcs11_logger_utils_str_to_long(env_var, &pkcs11_logger_flags))
        {
            pkcs11_logger_log("Unable to read the value of PKCS11_LOGGER_FLAGS environment variable as a number");
            goto err;
        }
    }

#ifdef _WIN32
#pragma warning(pop)
#endif

    rv = PKCS11_LOGGER_RV_SUCCESS;

err:

    if (rv == PKCS11_LOGGER_RV_ERROR)
    {
        CALL_N_CLEAR(free, pkcs11_logger_library_path);
        CALL_N_CLEAR(free, pkcs11_logger_log_file_path);
    }

    return rv;
}
