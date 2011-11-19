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


#ifdef _WIN32

static HANDLE g_lock = NULL;

#else

static pthread_mutex_t g_lock;

#endif


int pkcs11_logger_lock_create(void)
{
	int rv = PKCS11_LOGGER_RV_ERROR;

#ifdef _WIN32

	g_lock = CreateMutex(NULL, FALSE, NULL);
	if (NULL == g_lock)
	{
		pkcs11_logger_log("Unable to create lock");
		goto end;
	}

#else

	if (0 != pthread_mutex_init(&g_lock, NULL))
	{
		pkcs11_logger_log("Unable to create lock");
		goto end;
	}

#endif

	rv = PKCS11_LOGGER_RV_SUCCESS;

end:

	return rv;
}

void pkcs11_logger_lock_acquire(void)
{
#ifdef _WIN32

	if (WAIT_OBJECT_0 != WaitForSingleObject(g_lock, INFINITE))
		pkcs11_logger_log("Unable to get lock ownership");

#else

	if (0 != pthread_mutex_lock(&g_lock))
		pkcs11_logger_log("Unable to get lock ownership");

#endif
}

void pkcs11_logger_lock_release(void)
{
#ifdef _WIN32

	if (!ReleaseMutex(g_lock)) 
		pkcs11_logger_log("Unable to release lock ownership");

#else

	if (0 != pthread_mutex_unlock(&g_lock))
		pkcs11_logger_log("Unable to release lock ownership");

#endif
}

void pkcs11_logger_lock_destroy(void)
{
#ifdef _WIN32

	CALL_N_CLEAR(CloseHandle, g_lock);

#else

	pthread_mutex_destroy(&g_lock);

#endif
}
