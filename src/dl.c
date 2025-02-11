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


 // Platform dependend function that loads dynamic library
DLHANDLE pkcs11_logger_dl_open(const char* library)
{
    DLHANDLE handle = NULL;

    pkcs11_logger_log_with_timestamp("Going to load PKCS#11 library \"%s\"", library);

#ifdef _WIN32

    DWORD flags = 0;
    DWORD error = 0;

    if (CK_TRUE == pkcs11_logger_utils_path_is_absolute(library))
        flags = LOAD_WITH_ALTERED_SEARCH_PATH;

    handle = LoadLibraryExA(library, NULL, flags);
    if (NULL == handle)
    {
        error = GetLastError();
        pkcs11_logger_log_with_timestamp("Unable to load PKCS#11 library. Error: %0#10x", error);
    }
    else
    {
        pkcs11_logger_log_with_timestamp("Successfully loaded PKCS#11 library");
    }

#else

    char* error = NULL;

    handle = dlopen(library, RTLD_NOW | RTLD_LOCAL);
    if (NULL == handle)
    {
        error = dlerror();
        if (NULL != error)
        {
            pkcs11_logger_log_with_timestamp("Unable to load PKCS#11 library. Error: %s", error);
        }
        else
        {
            pkcs11_logger_log_with_timestamp("Unable to load PKCS#11 library");
        }
    }
    else
    {
        pkcs11_logger_log_with_timestamp("Successfully loaded PKCS#11 library");
    }

#endif

    return handle;
}


// Platform dependend function that gets function pointer from dynamic library
void *pkcs11_logger_dl_sym(DLHANDLE library, const char *function)
{
#ifdef _WIN32
    return (void*) GetProcAddress(library, function);
#else
    return dlsym(library, function);
#endif
}


// Platform dependend function that unloads dynamic library
int pkcs11_logger_dl_close(DLHANDLE library)
{
#ifdef _WIN32
    return FreeLibrary(library);
#else
    return dlclose(library);
#endif
}
