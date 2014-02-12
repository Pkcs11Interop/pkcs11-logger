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


// Converts string to long
int pkcs11_logger_utils_str_to_long(const char *str, unsigned long *val)
{
	unsigned long output = 0;
	char *endptr = NULL;

	if ((NULL == str) || (NULL == val))
		return PKCS11_LOGGER_RV_ERROR;

	errno = 0;
	output = strtoul(str, &endptr, 10);
	if ((0 != errno) || ('\0' != *endptr))
		return PKCS11_LOGGER_RV_ERROR;

	*val = output;

	return PKCS11_LOGGER_RV_SUCCESS;
}


// Gets current system time as string
void pkcs11_logger_utils_get_current_time_str(char* buff, int buff_len)
{
#ifdef _WIN32

	SYSTEMTIME systemtime;
	memset(&systemtime, 0, sizeof(SYSTEMTIME));

	memset(buff, 0, buff_len * sizeof(char));

	GetLocalTime(&systemtime);
	GetDateFormatA(LOCALE_SYSTEM_DEFAULT, 0, &systemtime, "yyyy-MM-dd ", buff, buff_len);
	GetTimeFormatA(LOCALE_SYSTEM_DEFAULT, 0, &systemtime, "HH:mm:ss", buff + 11, buff_len - 11);

#else

	struct timeval tv;
	struct tm tm;
	
	memset(buff, 0, buff_len * sizeof(char));

	if (gettimeofday(&tv, NULL) == 0)
		if (localtime_r(&tv.tv_sec, &tm) != NULL)
			strftime(buff, buff_len, "%Y-%m-%d %H:%M:%S", &tm);

#endif
}


// Gets ID of current thread
int pkcs11_logger_utils_get_thread_id()
{
#ifdef _WIN32
	return GetCurrentThreadId();
#else
#ifdef __APPLE__
	// TODO - This is ugly (but it works)
	return (int) (intptr_t) pthread_self();
#else
	return (int) pthread_self();
#endif
#endif
}


// Gets ID of current process
int pkcs11_logger_utils_get_process_id()
{
#ifdef _WIN32
	return _getpid();
#else
	return getpid();
#endif
}
