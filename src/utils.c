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
int pkcs11_logger_utils_get_thread_id(void)
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
int pkcs11_logger_utils_get_process_id(void)
{
#ifdef _WIN32
    return _getpid();
#else
    return getpid();
#endif
}
