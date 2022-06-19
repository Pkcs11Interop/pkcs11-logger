/*
 *  Copyright 2011-2016 The Pkcs11Interop Project
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


// Logs message
void pkcs11_logger_log(const char* message, ...)
{
    va_list ap;

    unsigned long disable_log_file = ((pkcs11_logger_globals.flags & PKCS11_LOGGER_FLAG_DISABLE_LOG_FILE) == PKCS11_LOGGER_FLAG_DISABLE_LOG_FILE);
    unsigned long disable_process_id = ((pkcs11_logger_globals.flags & PKCS11_LOGGER_FLAG_DISABLE_PROCESS_ID) == PKCS11_LOGGER_FLAG_DISABLE_PROCESS_ID);
    unsigned long disable_thread_id = ((pkcs11_logger_globals.flags & PKCS11_LOGGER_FLAG_DISABLE_THREAD_ID) == PKCS11_LOGGER_FLAG_DISABLE_THREAD_ID);
    unsigned long enable_stdout = ((pkcs11_logger_globals.flags & PKCS11_LOGGER_FLAG_ENABLE_STDOUT) == PKCS11_LOGGER_FLAG_ENABLE_STDOUT);
    unsigned long enable_stderr = ((pkcs11_logger_globals.flags & PKCS11_LOGGER_FLAG_ENABLE_STDERR) == PKCS11_LOGGER_FLAG_ENABLE_STDERR);
    unsigned long enable_fclose = ((pkcs11_logger_globals.flags & PKCS11_LOGGER_FLAG_ENABLE_FCLOSE) == PKCS11_LOGGER_FLAG_ENABLE_FCLOSE);

    // Acquire exclusive access to the file
    pkcs11_logger_lock_acquire();

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable: 4996)
#endif

    // Open log file
    if ((!disable_log_file) && (NULL != pkcs11_logger_globals.env_var_log_file_path) && (NULL == pkcs11_logger_globals.log_file_handle))
    {
        pkcs11_logger_globals.log_file_handle = fopen((const char *)pkcs11_logger_globals.env_var_log_file_path, "a");
    }

#ifdef _WIN32
#pragma warning(pop)
#endif

    // Log to file
    if ((!disable_log_file) && (NULL != pkcs11_logger_globals.log_file_handle))
    {
        va_start(ap, message);

        if (!disable_process_id)
            fprintf(pkcs11_logger_globals.log_file_handle, "%0#10x : ", pkcs11_logger_utils_get_process_id());
        if (!disable_thread_id)
            fprintf(pkcs11_logger_globals.log_file_handle, "%0#18lx : ", pkcs11_logger_utils_get_thread_id());
        vfprintf(pkcs11_logger_globals.log_file_handle, message, ap);
        fprintf(pkcs11_logger_globals.log_file_handle, "\n");

        va_end(ap);
    }

    // Log to stdout
    if (enable_stdout)
    {
        va_start(ap, message);

        if (!disable_process_id)
            fprintf(stdout, "%0#10x : ", pkcs11_logger_utils_get_process_id());
        if (!disable_thread_id)
            fprintf(stdout, "%0#18lx : ", pkcs11_logger_utils_get_thread_id());
        vfprintf(stdout, message, ap);
        fprintf(stdout, "\n");

        va_end(ap);
    }

    // Log to stderr
    if (enable_stderr || CK_FALSE == pkcs11_logger_globals.env_vars_read)
    {
        va_start(ap, message);

        if (!disable_process_id)
            fprintf(stderr, "%0#10x : ", pkcs11_logger_utils_get_process_id());
        if (!disable_thread_id)
            fprintf(stderr, "%0#18lx : ", pkcs11_logger_utils_get_thread_id());
        vfprintf(stderr, message, ap);
        fprintf(stderr, "\n");

        va_end(ap);
    }

    // Cleanup
    if (enable_fclose)
    {
        CALL_N_CLEAR(fclose, pkcs11_logger_globals.log_file_handle);
    }
    else
    {
        if (NULL != pkcs11_logger_globals.log_file_handle)
            fflush(pkcs11_logger_globals.log_file_handle);
    }
    
    // Release exclusive access to the file
    pkcs11_logger_lock_release();
}


// Logs separator line
void pkcs11_logger_log_separator(void)
{
    char str_time[20];
    pkcs11_logger_utils_get_current_time_str(str_time, sizeof(str_time));
    pkcs11_logger_log("****************************** %s ***", str_time);
}


// Logs function call
void pkcs11_logger_log_function_enter(const char *function)
{
    pkcs11_logger_log_separator();
    pkcs11_logger_log("Calling %s", function);
}


// Logs function exit
void pkcs11_logger_log_function_exit(CK_RV rv)
{
    pkcs11_logger_log("Returning %lu (%s)", rv, pkcs11_logger_translate_ck_rv(rv));
}


// Logs input params notice
void pkcs11_logger_log_input_params(void)
{
    pkcs11_logger_log("Input");
}


// Logs output params notice
void pkcs11_logger_log_output_params(void)
{
    pkcs11_logger_log("Output");
}


// Logs flag
void pkcs11_logger_log_flag(CK_ULONG flags, CK_ULONG flag_value, const char *flag_name)
{
    if (flags & flag_value)
        pkcs11_logger_log("%s: TRUE", flag_name);
    else
        pkcs11_logger_log("%s: FALSE", flag_name);
}


// Logs string that is not zero terminated
void pkcs11_logger_log_nonzero_string(const char *name, const CK_UTF8CHAR_PTR nonzero_string, CK_ULONG nonzero_string_len)
{
    if (NULL != nonzero_string)
    {
        unsigned char *zero_string = NULL;
        zero_string = (unsigned char*) malloc(nonzero_string_len + 1);
        if (NULL != zero_string)
        {
            memset(zero_string, 0, nonzero_string_len + 1);
            memcpy(zero_string, nonzero_string, nonzero_string_len);
            pkcs11_logger_log("%s: %s", name, zero_string);
            CALL_N_CLEAR(free, zero_string);
        }
        else
        {
            pkcs11_logger_log("%s: *** cannot be displayed ***", name);
        }
    }
}


// Logs byte array
void pkcs11_logger_log_byte_array(const char *name, CK_BYTE_PTR byte_array, CK_ULONG byte_array_len)
{
    if (NULL != byte_array)
    {
        char *array = NULL;
        array = pkcs11_logger_translate_ck_byte_ptr(byte_array, byte_array_len);
        if (NULL != array)
        {
            pkcs11_logger_log("%s: HEX(%s)", name, array);
            CALL_N_CLEAR(free, array);
        }
        else
        {
            pkcs11_logger_log("%s: *** cannot be displayed ***", name);
        }
    }
}


// Logs array of cryptoki attributes
void pkcs11_logger_log_attribute_template(CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount)
{
    CK_ULONG i = 0;
    
    if ((NULL == pTemplate) || (ulCount < 1))
        return;
    
    pkcs11_logger_log("  *** Begin attribute template ***");
    
    for (i = 0; i < ulCount; i++)
    {
        pkcs11_logger_log("  Attribute %d", i);
        pkcs11_logger_log("   Attribute: %lu (%s)", pTemplate[i].type, pkcs11_logger_translate_ck_attribute(pTemplate[i].type));
        pkcs11_logger_log("   pValue: %p", pTemplate[i].pValue);
        pkcs11_logger_log("   ulValueLen: %lu", pTemplate[i].ulValueLen);

        if (NULL != pTemplate[i].pValue)
        {
            char *value = NULL;

            if ((pTemplate[i].type & CKF_ARRAY_ATTRIBUTE) == CKF_ARRAY_ATTRIBUTE)
            {
                if (0 == (pTemplate[i].ulValueLen % sizeof(CK_ATTRIBUTE)))
                {
                    pkcs11_logger_log_attribute_template(pTemplate[i].pValue, pTemplate[i].ulValueLen / sizeof(CK_ATTRIBUTE));
                    continue;
                }
            }

            value = pkcs11_logger_translate_ck_byte_ptr(pTemplate[i].pValue, pTemplate[i].ulValueLen);
            if (NULL != value)
            {
                pkcs11_logger_log("   *pValue: HEX(%s)", value);
                CALL_N_CLEAR(free, value);
            }
            else
            {
                pkcs11_logger_log("   *pValue: *** cannot be displayed ***");
            }
        }
    }

    pkcs11_logger_log("  *** End attribute template ***");
}
