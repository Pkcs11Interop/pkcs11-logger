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

 
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>


#ifdef _WIN32

#include <windows.h>

#pragma pack(push, cryptoki, 1)

#define CK_PTR *
#define CK_DEFINE_FUNCTION(returnType, name) returnType name
#define CK_DECLARE_FUNCTION(returnType, name) returnType name
#define CK_DECLARE_FUNCTION_POINTER(returnType, name) returnType (* name)
#define CK_CALLBACK_FUNCTION(returnType, name) returnType (* name)

#ifndef NULL_PTR
#define NULL_PTR 0
#endif

#include <cryptoki\pkcs11.h>

#pragma pack(pop, cryptoki)

typedef HINSTANCE DLHANDLE;
#define DLOPEN(lib) LoadLibrary((lib))
#define DLSYM(lib, func) GetProcAddress((lib), (func))
#define DLCLOSE FreeLibrary

#define DEFAULT_PKCS11_LOGGER_LOG_FILE "c:\\pkcs11-logger.log"
#define DEFAULT_PKCS11_LOGGER_ORIG_LIB "opensc-pkcs11.dll"

#else // #ifdef _WIN32

#include <dlfcn.h>

#define CK_PTR *
#define CK_DEFINE_FUNCTION(returnType, name) returnType name
#define CK_DECLARE_FUNCTION(returnType, name) returnType name
#define CK_DECLARE_FUNCTION_POINTER(returnType, name) returnType (* name)
#define CK_CALLBACK_FUNCTION(returnType, name) returnType (* name)

#ifndef NULL_PTR
#define NULL_PTR 0
#endif

#include <cryptoki/pkcs11.h>

typedef void* DLHANDLE;
#define DLOPEN(lib) dlopen((lib), RTLD_NOW);
#define DLSYM(lib, func) dlsym((lib), (func))
#define DLCLOSE dlclose

#define DEFAULT_PKCS11_LOGGER_LOG_FILE "/tmp/pkcs11-logger.log"
#define DEFAULT_PKCS11_LOGGER_ORIG_LIB "opensc-pkcs11.so"

#endif // #ifdef _WIN32


#ifdef WIN32
#include <process.h>
#else // #ifdef WIN32
#include <pthread.h>
#include <unistd.h>
#endif //#ifdef WIN32


#define PKCS11_LOGGER_NAME "PCKS11-LOGGER"
#define PKCS11_LOGGER_VERSION "1.0"
#define PKCS11_LOGGER_DESCRIPTION "PKCS#11 logging proxy module"

#define PKCS11_LOGGER_RV_SUCCESS 1
#define PKCS11_LOGGER_RV_ERROR -1

#define CALL_N_CLEAR(function, pointer) do { if (NULL != pointer) { function(pointer); pointer = NULL; } } while(0)

int pkcs11_logger_lock_create(void);
void pkcs11_logger_lock_acquire(void);
void pkcs11_logger_lock_release(void);
void pkcs11_logger_lock_destroy(void);

void pkcs11_logger_log(const char* message, ...);
void pkcs11_logger_log_separator(void);
void pkcs11_logger_log_function_enter(const char *function);
void pkcs11_logger_log_function_exit(CK_RV rv);
void pkcs11_logger_log_input_params(void);
void pkcs11_logger_log_output_params(void);
void pkcs11_logger_log_flag(CK_ULONG flags, CK_ULONG flag_value, const char *flag_name);
void pkcs11_logger_log_nonzero_string(const char *name, const unsigned char *nonzero_string, CK_ULONG nonzero_string_len);
void pkcs11_logger_log_byte_array(const char *name, CK_BYTE_PTR byte_array, CK_ULONG byte_array_len);
void pkcs11_logger_log_attribute_template(CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount);

const char* pkcs11_logger_translate_ck_rv(CK_RV rv);
const char* pkcs11_logger_translate_ck_mechanism_type(CK_MECHANISM_TYPE type);
const char* pkcs11_logger_translate_ck_user_type(CK_USER_TYPE type);
const char* pkcs11_logger_translate_ck_state(CK_STATE state);
char* pkcs11_logger_translate_ck_byte_ptr(CK_BYTE_PTR bytes, CK_ULONG length);
const char* pkcs11_logger_translate_ck_attribute(CK_ATTRIBUTE_TYPE attribute);
