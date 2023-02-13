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


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>


#ifdef _WIN32


#include <windows.h>
#include <process.h>

// PKCS#11 related stuff
#pragma pack(push, cryptoki, 1)

#define CK_IMPORT_SPEC __declspec(dllimport) 

#ifdef CRYPTOKI_EXPORTS 
#define CK_EXPORT_SPEC __declspec(dllexport) 
#else 
#define CK_EXPORT_SPEC CK_IMPORT_SPEC 
#endif 

#define CK_CALL_SPEC __cdecl 

#define CK_PTR *
#define CK_DEFINE_FUNCTION(returnType, name) returnType CK_EXPORT_SPEC CK_CALL_SPEC name
#define CK_DECLARE_FUNCTION(returnType, name) returnType CK_EXPORT_SPEC CK_CALL_SPEC name
#define CK_DECLARE_FUNCTION_POINTER(returnType, name) returnType CK_IMPORT_SPEC (CK_CALL_SPEC CK_PTR name)
#define CK_CALLBACK_FUNCTION(returnType, name) returnType (CK_CALL_SPEC CK_PTR name)

#ifndef NULL_PTR
#define NULL_PTR 0
#endif

#include <cryptoki\pkcs11.h>

#pragma pack(pop, cryptoki)

// Platform dependend type for dynamically loaded library handle
typedef HINSTANCE DLHANDLE;
// Platform dependend function that loads dynamic library
#define DLOPEN(lib) LoadLibraryA((lib))
// Platform dependend function that gets function pointer from dynamic library
#define DLSYM(lib, func) GetProcAddress((lib), (func))
// Platform dependend function that unloads dynamic library
#define DLCLOSE FreeLibrary


#else // #ifdef _WIN32


#include <sys/time.h>
#include <dlfcn.h>
#include <pthread.h>
#include <unistd.h>
#include <strings.h>
#include <errno.h>

// PKCS#11 related stuff
#define CK_PTR *
#define CK_DEFINE_FUNCTION(returnType, name) returnType name
#define CK_DECLARE_FUNCTION(returnType, name) returnType name
#define CK_DECLARE_FUNCTION_POINTER(returnType, name) returnType (* name)
#define CK_CALLBACK_FUNCTION(returnType, name) returnType (* name)

#ifndef NULL_PTR
#define NULL_PTR 0
#endif

#include <cryptoki/pkcs11.h>

// Platform dependend type for dynamically loaded library handle
typedef void* DLHANDLE;
// Platform dependend function that loads dynamic library
#define DLOPEN(lib) dlopen((lib), RTLD_NOW | RTLD_LOCAL);
// Platform dependend function that gets function pointer from dynamic library
#define DLSYM(lib, func) dlsym((lib), (func))
// Platform dependend function that unloads dynamic library
#define DLCLOSE dlclose


#endif // #ifdef _WIN32


// Structure that holds global variables
typedef struct
{
    // Handle to original PKCS#11 library
    DLHANDLE orig_lib_handle;
    // Pointers to all cryptoki functions in original PKCS#11 library
    CK_FUNCTION_LIST_PTR orig_lib_functions;
    // Pointers to all cryptoki functions in PKCS11-LOGGER library
    CK_FUNCTION_LIST logger_functions;
    // Flag indicating whether environment variables has been successfuly read
    CK_BBOOL env_vars_read;
    // Value of PKCS11_LOGGER_LIBRARY_PATH environment variable
    CK_CHAR_PTR env_var_library_path;
    // Value of PKCS11_LOGGER_LOG_FILE_PATH environment variable
    CK_CHAR_PTR env_var_log_file_path;
    // Value of PKCS11_LOGGER_FLAGS environment variable
    CK_CHAR_PTR env_var_flags;
    // Value of PKCS11_LOGGER_FLAGS environment variable
    CK_ULONG flags;
    // Handle to log file
    FILE *log_file_handle;
}
PKCS11_LOGGER_GLOBALS;


// Environment variable that specifies path to the original PKCS#11 library
#define PKCS11_LOGGER_LIBRARY_PATH "PKCS11_LOGGER_LIBRARY_PATH"
// Environment variable that specifies path to the log file
#define PKCS11_LOGGER_LOG_FILE_PATH "PKCS11_LOGGER_LOG_FILE_PATH"
// Environment variable that specifies pkcs11-logger flags
#define PKCS11_LOGGER_FLAGS "PKCS11_LOGGER_FLAGS"

// Flag that disables logging into the log file
#define PKCS11_LOGGER_FLAG_DISABLE_LOG_FILE     0x00000001
// Flag that disables logging of process ID
#define PKCS11_LOGGER_FLAG_DISABLE_PROCESS_ID   0x00000002
// Flag that disables logging of thread ID
#define PKCS11_LOGGER_FLAG_DISABLE_THREAD_ID    0x00000004
// Flag that enables logging of PINs
#define PKCS11_LOGGER_FLAG_ENABLE_PIN           0x00000008
// Flag that enables logging to the stdout
#define PKCS11_LOGGER_FLAG_ENABLE_STDOUT        0x00000010
// Flag that enables logging to the stderr
#define PKCS11_LOGGER_FLAG_ENABLE_STDERR        0x00000020
// Flag that enables reopening of log file
#define PKCS11_LOGGER_FLAG_ENABLE_FCLOSE        0x00000040
// Flag that enabled more precise timestamping
#define PKCS11_LOGGER_FLAG_ENABLE_USECS         0x00000080

// Library name
#define PKCS11_LOGGER_NAME "PKCS11-LOGGER"
// Library version
#define PKCS11_LOGGER_VERSION "2.2.0"
// Library description
#define PKCS11_LOGGER_DESCRIPTION "PKCS#11 logging proxy module"

// Return value indicating success
#define PKCS11_LOGGER_RV_SUCCESS 1
// Return value indicating error
#define PKCS11_LOGGER_RV_ERROR -1

// Macro for safe resource clearing
#define CALL_N_CLEAR(function, pointer) if (NULL != pointer) { function(pointer); pointer = NULL; }
// Macro for safe initialization of original PKCS#11 library
#define SAFELY_INIT_ORIG_LIB_OR_FAIL() if (pkcs11_logger_init_orig_lib() != PKCS11_LOGGER_RV_SUCCESS) return CKR_GENERAL_ERROR;
// Macro that removes unused argument warning
#define IGNORE_ARG(P) (void)(P)

// init.c - declaration of functions
#ifdef _WIN32
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved);
#else
__attribute__((constructor)) void pkcs11_logger_init_entry_point(void);
__attribute__((destructor)) void pkcs11_logger_init_exit_point(void);
#endif
void pkcs11_logger_init_globals(void);
int pkcs11_logger_init_orig_lib(void);
int pkcs11_logger_init_parse_env_vars(void);
CK_CHAR_PTR pkcs11_logger_init_read_env_var(const char *env_var_name);

// lock.c - declaration of functions
int pkcs11_logger_lock_create(void);
void pkcs11_logger_lock_acquire(void);
void pkcs11_logger_lock_release(void);
void pkcs11_logger_lock_destroy(void);

// log.c - declaration of functions
void pkcs11_logger_log(const char* message, ...);
void pkcs11_logger_log_separator(void);
void pkcs11_logger_log_function_enter(const char *function);
void pkcs11_logger_log_function_exit(CK_RV rv);
void pkcs11_logger_log_input_params(void);
void pkcs11_logger_log_output_params(void);
void pkcs11_logger_log_flag(CK_ULONG flags, CK_ULONG flag_value, const char *flag_name);
void pkcs11_logger_log_nonzero_string(const char *name, const CK_UTF8CHAR_PTR nonzero_string, CK_ULONG nonzero_string_len);
void pkcs11_logger_log_byte_array(const char *name, CK_BYTE_PTR byte_array, CK_ULONG byte_array_len);
void pkcs11_logger_log_attribute_template(CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount);

// translate.c - declaration of functions
const char* pkcs11_logger_translate_ck_rv(CK_RV rv);
const char* pkcs11_logger_translate_ck_mechanism_type(CK_MECHANISM_TYPE type);
const char* pkcs11_logger_translate_ck_user_type(CK_USER_TYPE type);
const char* pkcs11_logger_translate_ck_state(CK_STATE state);
char* pkcs11_logger_translate_ck_byte_ptr(CK_BYTE_PTR bytes, CK_ULONG length);
const char* pkcs11_logger_translate_ck_attribute(CK_ATTRIBUTE_TYPE type);

// utils.c - declaration of functions
int pkcs11_logger_utils_str_to_long(const char *str, unsigned long *val);
void pkcs11_logger_utils_get_current_time_str(char* buff, int buff_len, unsigned long enable_usecs);
unsigned long pkcs11_logger_utils_get_thread_id(void);
int pkcs11_logger_utils_get_process_id(void);
