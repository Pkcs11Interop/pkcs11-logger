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
