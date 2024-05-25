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


// Lock for log file access synchronization
#ifdef _WIN32
static HANDLE pkcs11_logger_lock = NULL;
#else
static pthread_mutex_t pkcs11_logger_lock;
#endif


// Creates lock for log file access synchronization
int pkcs11_logger_lock_create(void)
{
    int rv = PKCS11_LOGGER_RV_ERROR;

#ifdef _WIN32

    if (NULL != pkcs11_logger_lock)
    {
        pkcs11_logger_log("Lock already exists");
        goto end;
    }

    pkcs11_logger_lock = CreateMutex(NULL, FALSE, NULL);
    if (NULL == pkcs11_logger_lock)
    {
        pkcs11_logger_log("Unable to create lock");
        goto end;
    }

#else

    if (0 != pthread_mutex_init(&pkcs11_logger_lock, NULL))
    {
        pkcs11_logger_log("Unable to create lock");
        goto end;
    }

#endif

    rv = PKCS11_LOGGER_RV_SUCCESS;

end:

    return rv;
}


// Acquires lock for log file access synchronization
void pkcs11_logger_lock_acquire(void)
{
#ifdef _WIN32

    if (NULL == pkcs11_logger_lock)
        return;

    if (WAIT_OBJECT_0 != WaitForSingleObject(pkcs11_logger_lock, INFINITE))
        pkcs11_logger_log("Unable to get lock ownership");

#else

    if (0 != pthread_mutex_lock(&pkcs11_logger_lock))
        pkcs11_logger_log("Unable to get lock ownership");

#endif
}


// Releases lock for log file access synchronization
void pkcs11_logger_lock_release(void)
{
#ifdef _WIN32

    if (NULL == pkcs11_logger_lock)
        return;

    if (!ReleaseMutex(pkcs11_logger_lock)) 
        pkcs11_logger_log("Unable to release lock ownership");

#else

    if (0 != pthread_mutex_unlock(&pkcs11_logger_lock))
        pkcs11_logger_log("Unable to release lock ownership");

#endif
}


// Destroys lock for log file access synchronization
void pkcs11_logger_lock_destroy(void)
{
#ifdef _WIN32

    CALL_N_CLEAR(CloseHandle, pkcs11_logger_lock);

#else

    pthread_mutex_destroy(&pkcs11_logger_lock);

#endif
}
