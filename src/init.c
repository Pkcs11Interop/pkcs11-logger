/*
 *  Copyright 2011-2024 The Pkcs11Interop Project
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 *  Written for the Pkcs11Interop project by:
 *  Jaroslav IMRICH <jimrich@jimrich.sk>
 */


#include "pkcs11-logger.h"


extern PKCS11_LOGGER_GLOBALS pkcs11_logger_globals;


#ifdef _WIN32

// Entry and exit point for the shared library on windows platforms
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    IGNORE_ARG(hModule);
    IGNORE_ARG(lpReserved);

    if ((DLL_PROCESS_ATTACH == ul_reason_for_call) || (DLL_PROCESS_DETACH == ul_reason_for_call))
        pkcs11_logger_init_globals();

    return TRUE;
}

#else

// Entry point for the shared library on unix platforms
__attribute__((constructor)) void pkcs11_logger_init_entry_point(void)
{
    pkcs11_logger_init_globals();
}

// Exit point for the shared library on unix platforms
__attribute__((destructor)) void pkcs11_logger_init_exit_point(void)
{
    pkcs11_logger_init_globals();
}

#endif


// Initializes/frees global variables
void pkcs11_logger_init_globals(void)
{
    // Note: Calling LoadLibrary() or FreeLibrary() in DllMain() can cause a deadlock or a crash.
    //       See "Dynamic-Link Library Best Practices" article on MSDN for more details.
    pkcs11_logger_globals.orig_lib_handle = NULL;
    pkcs11_logger_globals.orig_lib_functions = NULL;
    // Note: There is no need to modify pkcs11_logger_globals.logger_functions
    pkcs11_logger_globals.env_vars_read = CK_FALSE;
    CALL_N_CLEAR(free, pkcs11_logger_globals.env_var_library_path);
    CALL_N_CLEAR(free, pkcs11_logger_globals.env_var_log_file_path);
    CALL_N_CLEAR(free, pkcs11_logger_globals.env_var_flags);
    pkcs11_logger_globals.flags = 0;
    CALL_N_CLEAR(fclose, pkcs11_logger_globals.log_file_handle);
}


// Loads original PKCS#11 library
int pkcs11_logger_init_orig_lib(void)
{
    CK_C_GetFunctionList GetFunctionListPointer = NULL;
    CK_RV rv = CKR_OK;

    if (NULL != pkcs11_logger_globals.orig_lib_handle)
        return PKCS11_LOGGER_RV_SUCCESS;

    // Initialize global variables
    pkcs11_logger_init_globals();

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
    pkcs11_logger_globals.orig_lib_handle = pkcs11_logger_dl_open((const char *)pkcs11_logger_globals.env_var_library_path);
    if (NULL == pkcs11_logger_globals.orig_lib_handle)
    {
        return PKCS11_LOGGER_RV_ERROR;
    }

    // Get pointer to C_GetFunctionList()
    GetFunctionListPointer = (CK_C_GetFunctionList) pkcs11_logger_dl_sym(pkcs11_logger_globals.orig_lib_handle, "C_GetFunctionList");
    if (NULL == GetFunctionListPointer)
    {
        CALL_N_CLEAR(pkcs11_logger_dl_close, pkcs11_logger_globals.orig_lib_handle);
        return PKCS11_LOGGER_RV_ERROR;
    }

    // Get pointers to all PKCS#11 functions
    pkcs11_logger_log_with_timestamp("Going to call C_GetFunctionList function from the original library");
    rv = GetFunctionListPointer(&(pkcs11_logger_globals.orig_lib_functions));
    pkcs11_logger_log_with_timestamp("Received response from C_GetFunctionList function");
    if (CKR_OK != rv)
    {
        pkcs11_logger_log("C_GetFunctionList returned %lu (%s)", rv, pkcs11_logger_translate_ck_rv(rv));
        CALL_N_CLEAR(pkcs11_logger_dl_close, pkcs11_logger_globals.orig_lib_handle);
        return PKCS11_LOGGER_RV_ERROR;
    }

    // Lets present version of orig library as ours - that's what proxies do :)
    pkcs11_logger_globals.logger_functions.version.major = pkcs11_logger_globals.orig_lib_functions->version.major;
    pkcs11_logger_globals.logger_functions.version.minor = pkcs11_logger_globals.orig_lib_functions->version.minor;
    
    // Everything is set up
    pkcs11_logger_log_separator();
    pkcs11_logger_log("NOTE: Memory contents will be logged without the endianness conversion");

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
#ifdef _WIN32

    CK_CHAR_PTR output_value = NULL;
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

err:

    CALL_N_CLEAR(free, env_var_value);

    return output_value;

#else

    CK_CHAR_PTR output_value = NULL;
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

err:

    return output_value;

#endif
}

