/*
 *  PKCS11-LOGGER - PKCS#11 logging proxy module
 *  Copyright (c) 2011-2014 JWC s.r.o. <http://www.jwc.sk>
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


extern CK_BBOOL pkcs11_logger_env_vars_read;
extern CK_CHAR_PTR pkcs11_logger_log_file_path;
extern CK_ULONG pkcs11_logger_flags;


// Logs message
void pkcs11_logger_log(const char* message, ...)
{
	FILE *fw = NULL;
	va_list ap;

	unsigned long disable_log_file = ((pkcs11_logger_flags & PKCS11_LOGGER_FLAG_DISABLE_LOG_FILE) == PKCS11_LOGGER_FLAG_DISABLE_LOG_FILE);
	unsigned long disable_process_id = ((pkcs11_logger_flags & PKCS11_LOGGER_FLAG_DISABLE_PROCESS_ID) == PKCS11_LOGGER_FLAG_DISABLE_PROCESS_ID);
	unsigned long disable_thread_id = ((pkcs11_logger_flags & PKCS11_LOGGER_FLAG_DISABLE_THREAD_ID) == PKCS11_LOGGER_FLAG_DISABLE_THREAD_ID);
	unsigned long enable_stdout = ((pkcs11_logger_flags & PKCS11_LOGGER_FLAG_ENABLE_STDOUT) == PKCS11_LOGGER_FLAG_ENABLE_STDOUT);
	unsigned long enable_stderr = ((pkcs11_logger_flags & PKCS11_LOGGER_FLAG_ENABLE_STDERR) == PKCS11_LOGGER_FLAG_ENABLE_STDERR);

	// Acquire exclusive access to the file
	pkcs11_logger_lock_acquire();
	
#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable: 4996)
#endif

	// Open log file
	if (!disable_log_file && pkcs11_logger_log_file_path != NULL)
		fw = fopen((const char *) pkcs11_logger_log_file_path, "a");

#ifdef _WIN32
#pragma warning(pop)
#endif

	// Log to file
	if ((!disable_log_file) && (NULL != fw))
	{
		va_start(ap, message);

		if (!disable_process_id)
			fprintf(fw, "%0#10x : ", pkcs11_logger_utils_get_process_id());
		if (!disable_thread_id)
			fprintf(fw, "%0#10x : ", pkcs11_logger_utils_get_thread_id());
		vfprintf(fw, message, ap);
		fprintf(fw, "\n");

		va_end(ap);
	}

	// Log to stdout
	if (enable_stdout)
	{
		va_start(ap, message);

		if (!disable_process_id)
			fprintf(stdout, "%0#10x : ", pkcs11_logger_utils_get_process_id());
		if (!disable_thread_id)
			fprintf(stdout, "%0#10x : ", pkcs11_logger_utils_get_thread_id());
		vfprintf(stdout, message, ap);
		fprintf(stdout, "\n");
		
		va_end(ap);
	}

	// Log to stderr
	if (enable_stderr || pkcs11_logger_env_vars_read == CK_FALSE)
	{
		va_start(ap, message);

		if (!disable_process_id)
			fprintf(stderr, "%0#10x : ", pkcs11_logger_utils_get_process_id());
		if (!disable_thread_id)
			fprintf(stderr, "%0#10x : ", pkcs11_logger_utils_get_thread_id());
		vfprintf(stderr, message, ap);
		fprintf(stderr, "\n");

		va_end(ap);
	}
	
	// Cleanup
	CALL_N_CLEAR(fclose, fw);
	
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
