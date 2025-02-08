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
#ifdef _WIN32
    return LoadLibraryA(library);
#else
#ifdef __APPLE__
    return dlopen(library, RTLD_NOW | RTLD_LOCAL);
#else 
    return dlopen(library, RTLD_NOW | RTLD_LOCAL);
#endif
#endif
}


// Platform dependend function that gets function pointer from dynamic library
void *pkcs11_logger_dl_sym(DLHANDLE library, const char *function)
{
#ifdef _WIN32
    return GetProcAddress(library, function);
#else
#ifdef __APPLE__
    return dlsym(library, function);
#else 
    return dlsym(library, function);
#endif
#endif
}


// Platform dependend function that unloads dynamic library
int pkcs11_logger_dl_close(DLHANDLE library)
{
#ifdef _WIN32
    return FreeLibrary(library);
#else
#ifdef __APPLE__
    return dlclose(library);
#else 
    return dlclose(library);
#endif
#endif
}
