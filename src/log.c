/* PKCS11-LOGGER - PKCS#11 logging proxy module
 * Copyright (C) 2011 Jaroslav Imrich <jariq(at)jariq(dot)sk>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License versuin 3 
 * as published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "pkcs11-logger.h"


static void current_time_str(char* buff, int buff_len)
{
	memset(buff, 0, buff_len * sizeof(char));
	
#ifdef _WIN32

	struct timeval tv;

	if (gettimeofday(&tv, NULL) == 0)
		strftime(buff, buff_len, "%Y-%m-%d %H:%M:%S", localtime(&tv.tv_sec));

#else

	struct timeval tv;
	struct tm tm;
	
	if (gettimeofday(&tv, NULL) == 0)
		if (localtime_r(&tv.tv_sec, &tm) != NULL)
			strftime(buff, buff_len, "%Y-%m-%d %H:%M:%S", &tm);

#endif
}


static int get_thread_id()
{
#ifdef _WIN32
	return GetCurrentThreadId();
#else
	return pthread_self();
#endif
}

static int get_process_id()
{
#ifdef _WIN32
	return _getpid();
#else
	return getpid();
#endif
}


void pkcs11_logger_log(const char* message, ...)
{
	FILE *fw = NULL;
	FILE *output = stdout;
	va_list ap;
	
	// Acquire exclusive access to the file
	pkcs11_logger_lock_acquire();
	
	va_start(ap, message);
	
	// Determine log filename
	char *log_file_name = getenv("PKCS11_LOGGER_LOG_FILE");
	if (NULL == log_file_name)
		log_file_name = DEFAULT_PKCS11_LOGGER_LOG_FILE;
	
	// Open log file
	fw = fopen(log_file_name, "a");
	if (NULL != fw)
		output = fw;
	
	// Output message to log file or stdout
	fprintf(output, "%0#10x : ", get_process_id());
	fprintf(output, "%0#10x : ", get_thread_id());
	vfprintf(output, message, ap);
	fprintf(output, "\n");
	
	// Cleanup
	va_end(ap);
	CALL_N_CLEAR(fclose, fw);
	
	// Release exclusive access to the file
	pkcs11_logger_lock_release();
}


void pkcs11_logger_log_separator(void)
{
	char str_time[20];
	current_time_str(str_time, sizeof(str_time));
	pkcs11_logger_log("****************************** %s ***", str_time);
}


void pkcs11_logger_log_function_enter(const char *function)
{
	pkcs11_logger_log_separator();
	pkcs11_logger_log("Calling %s", function);
}


void pkcs11_logger_log_function_exit(CK_RV rv)
{
	pkcs11_logger_log("Returning %lu (%s)", rv, pkcs11_logger_translate_ck_rv(rv));
}


void pkcs11_logger_log_input_params(void)
{
	pkcs11_logger_log("Input");
}


void pkcs11_logger_log_output_params(void)
{
	pkcs11_logger_log("Output");
}
	

void pkcs11_logger_log_flag(CK_ULONG flags, CK_ULONG flag_value, const char *flag_name)
{
	if (flags & flag_value)
		pkcs11_logger_log("%s: TRUE", flag_name);
	else
		pkcs11_logger_log("%s: FALSE", flag_name);
}


void pkcs11_logger_log_nonzero_string(const char *name, const unsigned char *nonzero_string, CK_ULONG nonzero_string_len)
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


void pkcs11_logger_log_attribute_template(CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount)
{
	int i = 0;
	
	if ((NULL == pTemplate) || (ulCount < 1))
		return;
	
	for (i = 0; i < ulCount; i++)
	{
		pkcs11_logger_log("  Attribute %d", i);
		pkcs11_logger_log("   Attribute: %lu (%s)", pTemplate[i].type, pkcs11_logger_translate_ck_attribute(pTemplate[i].type));
		pkcs11_logger_log("   pValue: %p", pTemplate[i].pValue);
		pkcs11_logger_log("   ulValueLen: %lu", pTemplate[i].ulValueLen);

		if (NULL != pTemplate[i].pValue)
		{
			char *value = NULL;
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
}
