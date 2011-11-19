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


DLHANDLE g_orig_lib = NULL;


static CK_FUNCTION_LIST_PTR g_orig_lib_functions = NULL;


static CK_FUNCTION_LIST pkcs11_logger_functions = 
{
	{2, 20},
	&C_Initialize,
	&C_Finalize,
	&C_GetInfo,
	&C_GetFunctionList,
	&C_GetSlotList,
	&C_GetSlotInfo,
	&C_GetTokenInfo,
	&C_GetMechanismList,
	&C_GetMechanismInfo,
	&C_InitToken,
	&C_InitPIN,
	&C_SetPIN,
	&C_OpenSession,
	&C_CloseSession,
	&C_CloseAllSessions,
	&C_GetSessionInfo,
	&C_GetOperationState,
	&C_SetOperationState,
	&C_Login,
	&C_Logout,
	&C_CreateObject,
	&C_CopyObject,
	&C_DestroyObject,
	&C_GetObjectSize,
	&C_GetAttributeValue,
	&C_SetAttributeValue,
	&C_FindObjectsInit,
	&C_FindObjects,
	&C_FindObjectsFinal,
	&C_EncryptInit,
	&C_Encrypt,
	&C_EncryptUpdate,
	&C_EncryptFinal,
	&C_DecryptInit,
	&C_Decrypt,
	&C_DecryptUpdate,
	&C_DecryptFinal,
	&C_DigestInit,
	&C_Digest,
	&C_DigestUpdate,
	&C_DigestKey,
	&C_DigestFinal,
	&C_SignInit,
	&C_Sign,
	&C_SignUpdate,
	&C_SignFinal,
	&C_SignRecoverInit,
	&C_SignRecover,
	&C_VerifyInit,
	&C_Verify,
	&C_VerifyUpdate,
	&C_VerifyFinal,
	&C_VerifyRecoverInit,
	&C_VerifyRecover,
	&C_DigestEncryptUpdate,
	&C_DecryptDigestUpdate,
	&C_SignEncryptUpdate,
	&C_DecryptVerifyUpdate,
	&C_GenerateKey,
	&C_GenerateKeyPair,
	&C_WrapKey,
	&C_UnwrapKey,
	&C_DeriveKey,
	&C_SeedRandom,
	&C_GenerateRandom,
	&C_GetFunctionStatus,
	&C_CancelFunction,
	&C_WaitForSlotEvent
};


#define INIT_ORIG_LIB_OR_FAIL() if (safely_init_orig_lib() != PKCS11_LOGGER_RV_SUCCESS) return CKR_GENERAL_ERROR;


int safely_init_orig_lib(void)
{
	if (g_orig_lib != NULL)
		return PKCS11_LOGGER_RV_SUCCESS;

	// Determine original library name
	char *orig_lib = getenv("PKCS11_LOGGER_ORIG_LIB");
	if (orig_lib == NULL)
		orig_lib = DEFAULT_PKCS11_LOGGER_ORIG_LIB;
	
	g_orig_lib = DLOPEN(orig_lib);
	if (g_orig_lib == NULL)
	{
		pkcs11_logger_log_separator();
		pkcs11_logger_log("%s %s - Unable to load %s", PKCS11_LOGGER_NAME, PKCS11_LOGGER_VERSION, orig_lib);
		return PKCS11_LOGGER_RV_ERROR;
	}

	CK_C_GetFunctionList pGetFunctionList = (CK_C_GetFunctionList) DLSYM(g_orig_lib, "C_GetFunctionList");
	if (pGetFunctionList == NULL)
	{
		pkcs11_logger_log_separator();
		pkcs11_logger_log("%s %s - Unable to find C_GetFunctionList() in %s", PKCS11_LOGGER_NAME, PKCS11_LOGGER_VERSION, orig_lib);
		CALL_N_CLEAR(DLCLOSE, g_orig_lib);
		return PKCS11_LOGGER_RV_ERROR;
	}

	CK_RV rv = pGetFunctionList(&g_orig_lib_functions);
	if (rv != CKR_OK)
	{
		pkcs11_logger_log_separator();
		pkcs11_logger_log("%s %s - Unable to call C_GetFunctionList() from %s", PKCS11_LOGGER_NAME, PKCS11_LOGGER_VERSION, orig_lib);
		CALL_N_CLEAR(DLCLOSE, g_orig_lib);
		return PKCS11_LOGGER_RV_ERROR;
	}

	// Lets present version of orig library as ours - that's what proxies do :)
	pkcs11_logger_functions.version.major = g_orig_lib_functions->version.major;
	pkcs11_logger_functions.version.minor = g_orig_lib_functions->version.minor;
	
	// Create lock for synchronization of log file access
	if (PKCS11_LOGGER_RV_SUCCESS != pkcs11_logger_lock_create())
		return PKCS11_LOGGER_RV_ERROR;
	
	pkcs11_logger_log_separator();
	pkcs11_logger_log("%s %s - Successfuly loaded %s", PKCS11_LOGGER_NAME, PKCS11_LOGGER_VERSION, orig_lib);
	
	return PKCS11_LOGGER_RV_SUCCESS;
}


CK_DEFINE_FUNCTION(CK_RV, C_Initialize)(CK_VOID_PTR pInitArgs)
{
	INIT_ORIG_LIB_OR_FAIL();
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();

	pkcs11_logger_log(" pInitArgs: %p", pInitArgs);
	if (NULL != pInitArgs)
	{
		CK_C_INITIALIZE_ARGS *args = (CK_C_INITIALIZE_ARGS*) pInitArgs;
		pkcs11_logger_log("  CreateMutex: %p", args->CreateMutex);
		pkcs11_logger_log("  DestroyMutex: %p", args->DestroyMutex);
		pkcs11_logger_log("  LockMutex: %p", args->LockMutex);
		pkcs11_logger_log("  UnlockMutex: %p", args->UnlockMutex);
		pkcs11_logger_log("  Flags: %lu", args->flags);
		pkcs11_logger_log_flag(args->flags, CKF_LIBRARY_CANT_CREATE_OS_THREADS, "   CKF_LIBRARY_CANT_CREATE_OS_THREADS");
		pkcs11_logger_log_flag(args->flags, CKF_OS_LOCKING_OK, "   CKF_OS_LOCKING_OK");
		pkcs11_logger_log("  pReserved: %p", args->pReserved);
	}
	
	CK_RV rv = g_orig_lib_functions->C_Initialize(pInitArgs);

	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_Finalize)(CK_VOID_PTR pReserved)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" pReserved: %p", pReserved);
	
	CK_RV rv = g_orig_lib_functions->C_Finalize(pReserved);
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_GetInfo)(CK_INFO_PTR pInfo)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" pInfo: %p", pInfo);
	
	CK_RV rv = g_orig_lib_functions->C_GetInfo(pInfo);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();

		pkcs11_logger_log(" pInfo: %p", pInfo);
		if (NULL != pInfo)
		{
			pkcs11_logger_log("  cryptokiVersion:");
			pkcs11_logger_log("   major: %d", pInfo->cryptokiVersion.major);
			pkcs11_logger_log("   minor: %d", pInfo->cryptokiVersion.minor);
			pkcs11_logger_log("  manufacturerID: %.32s", pInfo->manufacturerID);
			pkcs11_logger_log("  flags: %lu", pInfo->flags);
			pkcs11_logger_log("  libraryDescription: %.32s", pInfo->libraryDescription);		
			pkcs11_logger_log("  libraryVersion:");
			pkcs11_logger_log("   major: %d", pInfo->libraryVersion.major);
			pkcs11_logger_log("   minor: %d", pInfo->libraryVersion.minor);		
		}
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_GetFunctionList)(CK_FUNCTION_LIST_PTR_PTR ppFunctionList)
{
	INIT_ORIG_LIB_OR_FAIL();
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" ppFunctionList: %p", ppFunctionList);
	
	*ppFunctionList = &pkcs11_logger_functions;
	
	pkcs11_logger_log_output_params();
	
	pkcs11_logger_log(" Note: Returning function list of %s", PKCS11_LOGGER_NAME);
	
	CK_RV rv = CKR_OK;
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_GetSlotList)(CK_BBOOL tokenPresent, CK_SLOT_ID_PTR pSlotList, CK_ULONG_PTR pulCount)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	int i = 0;
	
	pkcs11_logger_log(" tokenPresent: %d", tokenPresent);
	pkcs11_logger_log(" pSlotList: %p", pSlotList);
	pkcs11_logger_log(" pulCount: %p", pulCount);
	if (NULL != pulCount)
		pkcs11_logger_log(" *pulCount: %lu", *pulCount);

	CK_RV rv = g_orig_lib_functions->C_GetSlotList(tokenPresent, pSlotList, pulCount);

	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();

		pkcs11_logger_log(" pSlotList: %p", pSlotList);
		if (NULL != pSlotList)
		{
			for (i = 0; i < *pulCount; i++)
			{
				pkcs11_logger_log(" pSlotList[%d]: %lu", i, pSlotList[i]);
			}
		}
		
		pkcs11_logger_log(" pulCount: %p", pulCount);
		if (NULL != pulCount)
			pkcs11_logger_log(" *pulCount: %lu", *pulCount);
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_GetSlotInfo)(CK_SLOT_ID slotID, CK_SLOT_INFO_PTR pInfo)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" slotID: %lu", slotID);
	pkcs11_logger_log(" pInfo: %p", pInfo);
	
	CK_RV rv = g_orig_lib_functions->C_GetSlotInfo(slotID, pInfo);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();

		pkcs11_logger_log(" pInfo: %p", pInfo);
		if (NULL != pInfo)
		{
			pkcs11_logger_log("  slotDescription: %.64s", pInfo->slotDescription);
			pkcs11_logger_log("  manufacturerID: %.32s", pInfo->manufacturerID);
			pkcs11_logger_log("  flags: %lu", pInfo->flags);
			pkcs11_logger_log_flag(pInfo->flags, CKF_TOKEN_PRESENT, "   CKF_TOKEN_PRESENT");
			pkcs11_logger_log_flag(pInfo->flags, CKF_REMOVABLE_DEVICE, "   CKF_REMOVABLE_DEVICE");
			pkcs11_logger_log_flag(pInfo->flags, CKF_HW_SLOT, "   CKF_HW_SLOT");
			pkcs11_logger_log("  hardwareVersion:");
			pkcs11_logger_log("   major: %d", pInfo->hardwareVersion.major);
			pkcs11_logger_log("   minor: %d", pInfo->hardwareVersion.minor);
			pkcs11_logger_log("  firmwareVersion:");
			pkcs11_logger_log("   major: %d", pInfo->firmwareVersion.major);
			pkcs11_logger_log("   minor: %d", pInfo->firmwareVersion.minor);
		}
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_GetTokenInfo)(CK_SLOT_ID slotID, CK_TOKEN_INFO_PTR pInfo)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" slotID: %lu", slotID);
	pkcs11_logger_log(" pInfo: %p", pInfo);
	
	CK_RV rv = g_orig_lib_functions->C_GetTokenInfo(slotID, pInfo);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();

		pkcs11_logger_log(" pInfo: %p", pInfo);
		if (NULL != pInfo)
		{
			pkcs11_logger_log("  label: %.32s", pInfo->label);
			pkcs11_logger_log("  manufacturerID: %.32s", pInfo->manufacturerID);
			pkcs11_logger_log("  model: %.16s", pInfo->model);
			pkcs11_logger_log("  serialNumber: %.16s", pInfo->serialNumber);
			pkcs11_logger_log("  flags: %lu", pInfo->flags);
			pkcs11_logger_log_flag(pInfo->flags, CKF_RNG, "   CKF_RNG");
			pkcs11_logger_log_flag(pInfo->flags, CKF_WRITE_PROTECTED, "   CKF_WRITE_PROTECTED");
			pkcs11_logger_log_flag(pInfo->flags, CKF_LOGIN_REQUIRED, "   CKF_LOGIN_REQUIRED");
			pkcs11_logger_log_flag(pInfo->flags, CKF_USER_PIN_INITIALIZED, "   CKF_USER_PIN_INITIALIZED");
			pkcs11_logger_log_flag(pInfo->flags, CKF_RESTORE_KEY_NOT_NEEDED, "   CKF_RESTORE_KEY_NOT_NEEDED");
			pkcs11_logger_log_flag(pInfo->flags, CKF_CLOCK_ON_TOKEN, "   CKF_CLOCK_ON_TOKEN");
			pkcs11_logger_log_flag(pInfo->flags, CKF_PROTECTED_AUTHENTICATION_PATH, "   CKF_PROTECTED_AUTHENTICATION_PATH");
			pkcs11_logger_log_flag(pInfo->flags, CKF_DUAL_CRYPTO_OPERATIONS, "   CKF_DUAL_CRYPTO_OPERATIONS");
			pkcs11_logger_log_flag(pInfo->flags, CKF_TOKEN_INITIALIZED, "   CKF_TOKEN_INITIALIZED");
			pkcs11_logger_log_flag(pInfo->flags, CKF_SECONDARY_AUTHENTICATION, "   CKF_SECONDARY_AUTHENTICATION");
			pkcs11_logger_log_flag(pInfo->flags, CKF_USER_PIN_COUNT_LOW, "   CKF_USER_PIN_COUNT_LOW");
			pkcs11_logger_log_flag(pInfo->flags, CKF_USER_PIN_FINAL_TRY, "   CKF_USER_PIN_FINAL_TRY");
			pkcs11_logger_log_flag(pInfo->flags, CKF_USER_PIN_LOCKED, "   CKF_USER_PIN_LOCKED");
			pkcs11_logger_log_flag(pInfo->flags, CKF_USER_PIN_TO_BE_CHANGED, "   CKF_USER_PIN_TO_BE_CHANGED");
			pkcs11_logger_log_flag(pInfo->flags, CKF_SO_PIN_COUNT_LOW, "   CKF_SO_PIN_COUNT_LOW");
			pkcs11_logger_log_flag(pInfo->flags, CKF_SO_PIN_FINAL_TRY, "   CKF_SO_PIN_FINAL_TRY");
			pkcs11_logger_log_flag(pInfo->flags, CKF_SO_PIN_LOCKED, "   CKF_SO_PIN_LOCKED");
			pkcs11_logger_log_flag(pInfo->flags, CKF_SO_PIN_TO_BE_CHANGED, "   CKF_SO_PIN_TO_BE_CHANGED");
			pkcs11_logger_log("  ulMaxSessionCount: %lu", pInfo->ulMaxSessionCount);
			pkcs11_logger_log("  ulSessionCount: %lu", pInfo->ulSessionCount);
			pkcs11_logger_log("  ulMaxRwSessionCount: %lu", pInfo->ulMaxRwSessionCount);
			pkcs11_logger_log("  ulRwSessionCount: %lu", pInfo->ulRwSessionCount);
			pkcs11_logger_log("  ulMaxPinLen: %lu", pInfo->ulMaxPinLen);
			pkcs11_logger_log("  ulMinPinLen: %lu", pInfo->ulMinPinLen);
			pkcs11_logger_log("  ulTotalPublicMemory: %lu", pInfo->ulTotalPublicMemory);
			pkcs11_logger_log("  ulFreePublicMemory: %lu", pInfo->ulFreePublicMemory);
			pkcs11_logger_log("  ulTotalPrivateMemory: %lu", pInfo->ulTotalPrivateMemory);
			pkcs11_logger_log("  ulFreePrivateMemory: %lu", pInfo->ulFreePrivateMemory);
			pkcs11_logger_log("  hardwareVersion:");
			pkcs11_logger_log("   major: %d", pInfo->hardwareVersion.major);
			pkcs11_logger_log("   minor: %d", pInfo->hardwareVersion.minor);
			pkcs11_logger_log("  firmwareVersion:");
			pkcs11_logger_log("   major: %d", pInfo->firmwareVersion.major);
			pkcs11_logger_log("   minor: %d", pInfo->firmwareVersion.minor);
			pkcs11_logger_log("  utcTime: %.16s", pInfo->utcTime);
		}
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_GetMechanismList)(CK_SLOT_ID slotID, CK_MECHANISM_TYPE_PTR pMechanismList, CK_ULONG_PTR pulCount)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();

	int i = 0;
	
	pkcs11_logger_log(" slotID: %lu", slotID);
	pkcs11_logger_log(" pMechanismList: %p", pMechanismList);
	pkcs11_logger_log(" pulCount: %p", pulCount);
	if (NULL != pulCount)
		pkcs11_logger_log(" *pulCount: %lu", *pulCount);
	
	CK_RV rv = g_orig_lib_functions->C_GetMechanismList(slotID, pMechanismList, pulCount);

	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();

		pkcs11_logger_log(" pMechanismList: %p", pMechanismList);			
		if (NULL != pMechanismList)
		{
			for (i = 0; i < *pulCount; i++)
			{
				pkcs11_logger_log("  pMechanismList[%d]: %lu (%s)", i, pMechanismList[i], pkcs11_logger_translate_ck_mechanism_type(pMechanismList[i]));
			}
		}
		
		pkcs11_logger_log(" pulCount: %p", pulCount);
		if (NULL != pulCount)
			pkcs11_logger_log(" *pulCount: %lu", *pulCount);
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_GetMechanismInfo)(CK_SLOT_ID slotID, CK_MECHANISM_TYPE type, CK_MECHANISM_INFO_PTR pInfo)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" slotID: %lu", slotID);
	pkcs11_logger_log(" type: %lu (%s)", type, pkcs11_logger_translate_ck_mechanism_type(type));
	pkcs11_logger_log(" pInfo: %p", pInfo);
	
	CK_RV rv = g_orig_lib_functions->C_GetMechanismInfo(slotID, type, pInfo);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();

		pkcs11_logger_log(" pInfo: %p", pInfo);
		if (NULL != pInfo)
		{
			pkcs11_logger_log("  ulMinKeySize: %lu", pInfo->ulMinKeySize);
			pkcs11_logger_log("  ulMaxKeySize: %lu", pInfo->ulMaxKeySize);
			pkcs11_logger_log("  flags: %lu", pInfo->flags);
			pkcs11_logger_log_flag(pInfo->flags, CKF_HW, "   CKF_HW");
			pkcs11_logger_log_flag(pInfo->flags, CKF_ENCRYPT, "   CKF_ENCRYPT");
			pkcs11_logger_log_flag(pInfo->flags, CKF_DECRYPT, "   CKF_DECRYPT");
			pkcs11_logger_log_flag(pInfo->flags, CKF_DIGEST, "   CKF_DIGEST");
			pkcs11_logger_log_flag(pInfo->flags, CKF_SIGN, "   CKF_SIGN");
			pkcs11_logger_log_flag(pInfo->flags, CKF_SIGN_RECOVER, "   CKF_SIGN_RECOVER");
			pkcs11_logger_log_flag(pInfo->flags, CKF_VERIFY, "   CKF_VERIFY");
			pkcs11_logger_log_flag(pInfo->flags, CKF_VERIFY_RECOVER, "   CKF_VERIFY_RECOVER");
			pkcs11_logger_log_flag(pInfo->flags, CKF_GENERATE, "   CKF_GENERATE");
			pkcs11_logger_log_flag(pInfo->flags, CKF_GENERATE_KEY_PAIR, "   CKF_GENERATE_KEY_PAIR");
			pkcs11_logger_log_flag(pInfo->flags, CKF_WRAP, "   CKF_WRAP");
			pkcs11_logger_log_flag(pInfo->flags, CKF_UNWRAP, "   CKF_UNWRAP");
			pkcs11_logger_log_flag(pInfo->flags, CKF_DERIVE, "   CKF_DERIVE");
			pkcs11_logger_log_flag(pInfo->flags, CKF_EC_F_P, "   CKF_EC_F_P");
			pkcs11_logger_log_flag(pInfo->flags, CKF_EC_F_2M, "   CKF_EC_F_2M");
			pkcs11_logger_log_flag(pInfo->flags, CKF_EC_ECPARAMETERS, "   CKF_EC_ECPARAMETERS");
			pkcs11_logger_log_flag(pInfo->flags, CKF_EC_NAMEDCURVE, "   CKF_EC_NAMEDCURVE");
			pkcs11_logger_log_flag(pInfo->flags, CKF_EC_UNCOMPRESS, "   CKF_EC_UNCOMPRESS");
			pkcs11_logger_log_flag(pInfo->flags, CKF_EC_COMPRESS, "   CKF_EC_COMPRESS");
			pkcs11_logger_log_flag(pInfo->flags, CKF_EXTENSION, "   CKF_EXTENSION");
		}
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_InitToken)(CK_SLOT_ID slotID, CK_UTF8CHAR_PTR pPin, CK_ULONG ulPinLen, CK_UTF8CHAR_PTR pLabel)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" slotID: %lu", slotID);
	pkcs11_logger_log(" pPin: %p", pPin);
	pkcs11_logger_log_nonzero_string(" *pPin", pPin, ulPinLen);
	pkcs11_logger_log(" ulPinLen: %lu", ulPinLen);
	pkcs11_logger_log(" pLabel: %p", pLabel);
	if (NULL != pLabel)
		pkcs11_logger_log(" *pLabel: %s", pLabel);
	
	CK_RV rv = g_orig_lib_functions->C_InitToken(slotID, pPin, ulPinLen, pLabel);
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_InitPIN)(CK_SESSION_HANDLE hSession, CK_UTF8CHAR_PTR pPin, CK_ULONG ulPinLen)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pPin: %p", pPin);
	pkcs11_logger_log_nonzero_string(" *pPin", pPin, ulPinLen);
	pkcs11_logger_log(" ulPinLen: %lu", ulPinLen);

	CK_RV rv = g_orig_lib_functions->C_InitPIN(hSession, pPin, ulPinLen);
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_SetPIN)(CK_SESSION_HANDLE hSession, CK_UTF8CHAR_PTR pOldPin, CK_ULONG ulOldLen, CK_UTF8CHAR_PTR pNewPin, CK_ULONG ulNewLen)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pOldPin: %p", pOldPin);
	pkcs11_logger_log_nonzero_string(" *pOldPin", pOldPin, ulOldLen);
	pkcs11_logger_log(" ulOldLen: %lu", ulOldLen);
	pkcs11_logger_log(" pNewPin: %p", pNewPin);
	pkcs11_logger_log_nonzero_string(" *pNewPin", pNewPin, ulNewLen);
	pkcs11_logger_log(" ulNewLen: %lu", ulNewLen);
	
	CK_RV rv = g_orig_lib_functions->C_SetPIN(hSession, pOldPin, ulOldLen, pNewPin, ulNewLen);
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_OpenSession)(CK_SLOT_ID slotID, CK_FLAGS flags, CK_VOID_PTR pApplication, CK_NOTIFY Notify, CK_SESSION_HANDLE_PTR phSession)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();	
	
	pkcs11_logger_log(" slotID: %lu", slotID);
	pkcs11_logger_log(" flags: %lu", flags);
	pkcs11_logger_log_flag(flags, CKF_RW_SESSION, "  CKF_RW_SESSION");
	pkcs11_logger_log_flag(flags, CKF_SERIAL_SESSION, "  CKF_SERIAL_SESSION");
	pkcs11_logger_log(" pApplication: %p", pApplication);
	pkcs11_logger_log(" Notify: %p", Notify);
	pkcs11_logger_log(" phSession: %p", phSession);
	if (NULL != phSession)
		pkcs11_logger_log(" *phSession: %lu", phSession);
	
	CK_RV rv = g_orig_lib_functions->C_OpenSession(slotID, flags, pApplication, Notify, phSession);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();
		
		pkcs11_logger_log(" phSession: %p", phSession);
		if (NULL != phSession)
			pkcs11_logger_log(" *phSession: %lu", *phSession);
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_CloseSession)(CK_SESSION_HANDLE hSession)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();	
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	
	CK_RV rv = g_orig_lib_functions->C_CloseSession(hSession);
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_CloseAllSessions)(CK_SLOT_ID slotID)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();	
	
	pkcs11_logger_log(" slotID: %lu", slotID);
	
	CK_RV rv = g_orig_lib_functions->C_CloseAllSessions(slotID);
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_GetSessionInfo)(CK_SESSION_HANDLE hSession, CK_SESSION_INFO_PTR pInfo)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();	
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pInfo: %p", pInfo);
	
	CK_RV rv = g_orig_lib_functions->C_GetSessionInfo(hSession, pInfo);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();
		
		pkcs11_logger_log(" pInfo: %p", pInfo);
		if (NULL != pInfo)
		{
			pkcs11_logger_log("  slotID: %lu", pInfo->slotID);
			pkcs11_logger_log("  state: %lu (%s)", pInfo->state, pkcs11_logger_translate_ck_state(pInfo->state));
			pkcs11_logger_log("  flags: %lu", pInfo->flags);
			pkcs11_logger_log_flag(pInfo->flags, CKF_RW_SESSION, "   CKF_RW_SESSION");
			pkcs11_logger_log_flag(pInfo->flags, CKF_SERIAL_SESSION, "   CKF_SERIAL_SESSION");
			pkcs11_logger_log("  ulDeviceError: %lu", pInfo->ulDeviceError);
		}
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_GetOperationState)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pOperationState, CK_ULONG_PTR pulOperationStateLen)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();	
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pOperationState: %p", pOperationState);
	pkcs11_logger_log(" pulOperationStateLen: %p", pulOperationStateLen);
	if (NULL != pulOperationStateLen)
		pkcs11_logger_log(" *pulOperationStateLen: %lu", *pulOperationStateLen);
	
	CK_RV rv = g_orig_lib_functions->C_GetOperationState(hSession, pOperationState, pulOperationStateLen);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();
		
		pkcs11_logger_log(" pOperationState: %p", pOperationState);
		if (NULL != pulOperationStateLen)
			pkcs11_logger_log_byte_array(" *pOperationState", pOperationState, *pulOperationStateLen);
		pkcs11_logger_log(" pulOperationStateLen: %p", pulOperationStateLen);
		if (NULL != pulOperationStateLen)
			pkcs11_logger_log(" *pulOperationStateLen: %lu", *pulOperationStateLen);
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_SetOperationState)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pOperationState, CK_ULONG ulOperationStateLen, CK_OBJECT_HANDLE hEncryptionKey, CK_OBJECT_HANDLE hAuthenticationKey)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();	
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pOperationState: %p", pOperationState);
	pkcs11_logger_log_byte_array(" *pOperationState", pOperationState, ulOperationStateLen);
	pkcs11_logger_log(" ulOperationStateLen: %lu", ulOperationStateLen);
	pkcs11_logger_log(" hEncryptionKey: %lu", hEncryptionKey);
	pkcs11_logger_log(" hAuthenticationKey: %lu", hAuthenticationKey);
	
	CK_RV rv = g_orig_lib_functions->C_SetOperationState(hSession, pOperationState, ulOperationStateLen, hEncryptionKey, hAuthenticationKey);
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_Login)(CK_SESSION_HANDLE hSession, CK_USER_TYPE userType, CK_UTF8CHAR_PTR pPin, CK_ULONG ulPinLen)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" userType: %lu (%s)", userType, pkcs11_logger_translate_ck_user_type(userType));
	pkcs11_logger_log(" pPin: %p", pPin);
	pkcs11_logger_log_nonzero_string(" *pPin", pPin, ulPinLen);
	pkcs11_logger_log(" ulPinLen: %lu", ulPinLen);
	
	CK_RV rv = g_orig_lib_functions->C_Login(hSession, userType, pPin, ulPinLen);
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_Logout)(CK_SESSION_HANDLE hSession)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	
	CK_RV rv = g_orig_lib_functions->C_Logout(hSession);
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_CreateObject)(CK_SESSION_HANDLE hSession, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_OBJECT_HANDLE_PTR phObject)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pTemplate: %p", pTemplate);
	pkcs11_logger_log(" ulCount: %lu", ulCount);
	pkcs11_logger_log_attribute_template(pTemplate, ulCount);
	pkcs11_logger_log(" phObject: %p", phObject);
	if (NULL != phObject)
		pkcs11_logger_log(" *phObject: %lu", *phObject);
	
	CK_RV rv = g_orig_lib_functions->C_CreateObject(hSession, pTemplate, ulCount, phObject);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();
		
		pkcs11_logger_log(" phObject: %p", phObject);
		if (NULL != phObject)
			pkcs11_logger_log(" *phObject: %lu", *phObject);
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_CopyObject)(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hObject, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_OBJECT_HANDLE_PTR phNewObject)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" hObject: %lu", hObject);
	pkcs11_logger_log(" pTemplate: %p", pTemplate);
	pkcs11_logger_log(" ulCount: %lu", ulCount);
	pkcs11_logger_log_attribute_template(pTemplate, ulCount);
	pkcs11_logger_log(" phNewObject: %p", phNewObject);
	if (NULL != phNewObject)
		pkcs11_logger_log(" *phNewObject: %lu", *phNewObject);
	
	CK_RV rv = g_orig_lib_functions->C_CopyObject(hSession, hObject, pTemplate, ulCount, phNewObject);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();
		
		pkcs11_logger_log(" phNewObject: %p", phNewObject);
		if (NULL != phNewObject)
			pkcs11_logger_log(" *phNewObject: %lu", *phNewObject);
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_DestroyObject)(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hObject)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" hObject: %lu", hObject);
	
	CK_RV rv = g_orig_lib_functions->C_DestroyObject(hSession, hObject);
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_GetObjectSize)(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hObject, CK_ULONG_PTR pulSize)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" hObject: %lu", hObject);
	pkcs11_logger_log(" pulSize: %p", pulSize);
	if (NULL != pulSize)
		pkcs11_logger_log(" *pulSize: %lu", *pulSize);
	
	CK_RV rv = g_orig_lib_functions->C_GetObjectSize(hSession, hObject, pulSize);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();
		
		pkcs11_logger_log(" pulSize: %p", pulSize);
		if (NULL != pulSize)
			pkcs11_logger_log(" *pulSize: %lu", *pulSize);
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_GetAttributeValue)(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hObject, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" hObject: %lu", hObject);
	pkcs11_logger_log(" pTemplate: %p", pTemplate);
	pkcs11_logger_log(" ulCount: %lu", ulCount);
	pkcs11_logger_log_attribute_template(pTemplate, ulCount);
	
	CK_RV rv = g_orig_lib_functions->C_GetAttributeValue(hSession, hObject, pTemplate, ulCount);
	
	if ((CKR_OK == rv) || (CKR_ATTRIBUTE_SENSITIVE == rv) || (CKR_ATTRIBUTE_TYPE_INVALID == rv) || (CKR_BUFFER_TOO_SMALL == rv))
	{
		pkcs11_logger_log_output_params();
		
		pkcs11_logger_log(" pTemplate: %p", pTemplate);
		pkcs11_logger_log(" ulCount: %lu", ulCount);
		pkcs11_logger_log_attribute_template(pTemplate, ulCount);
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_SetAttributeValue)(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hObject, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" hObject: %lu", hObject);
	pkcs11_logger_log(" pTemplate: %p", pTemplate);
	pkcs11_logger_log(" ulCount: %lu", ulCount);
	pkcs11_logger_log_attribute_template(pTemplate, ulCount);	
	
	CK_RV rv = g_orig_lib_functions->C_SetAttributeValue(hSession, hObject, pTemplate, ulCount);
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_FindObjectsInit)(CK_SESSION_HANDLE hSession, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pTemplate: %p", pTemplate);
	pkcs11_logger_log(" ulCount: %lu", ulCount);
	pkcs11_logger_log_attribute_template(pTemplate, ulCount);		
	
	CK_RV rv = g_orig_lib_functions->C_FindObjectsInit(hSession, pTemplate, ulCount);
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_FindObjects)(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE_PTR phObject, CK_ULONG ulMaxObjectCount, CK_ULONG_PTR pulObjectCount)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	int i = 0;
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" phObject: %p", phObject);
	pkcs11_logger_log(" ulMaxObjectCount: %lu", ulMaxObjectCount);
	pkcs11_logger_log(" pulObjectCount: %p", pulObjectCount);
	if (NULL != pulObjectCount)
		pkcs11_logger_log(" *pulObjectCount: %lu", *pulObjectCount);	
	
	if ((NULL != phObject) && (ulMaxObjectCount > 0))
	{
		for (i = 0; i < ulMaxObjectCount; i++)
		{
			pkcs11_logger_log("  *phObject[%d]: %lu", i, phObject[i]);
		}
	}
	
	CK_RV rv = g_orig_lib_functions->C_FindObjects(hSession, phObject, ulMaxObjectCount, pulObjectCount);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();
		
		pkcs11_logger_log(" phObject: %p", phObject);
		pkcs11_logger_log(" ulMaxObjectCount: %lu", ulMaxObjectCount);
		pkcs11_logger_log(" pulObjectCount: %p", pulObjectCount);
		if (NULL != pulObjectCount)
			pkcs11_logger_log(" *pulObjectCount: %lu", *pulObjectCount);
		
		if ((NULL != phObject) && (ulMaxObjectCount > 0))
		{
			for (i = 0; i < ulMaxObjectCount; i++)
			{
				pkcs11_logger_log("  *phObject[%d]: %lu", i, phObject[i]);
			}
		}
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_FindObjectsFinal)(CK_SESSION_HANDLE hSession)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	
	CK_RV rv = g_orig_lib_functions->C_FindObjectsFinal(hSession);
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_EncryptInit)(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hKey)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pMechanism: %p", pMechanism);
	if (NULL != pMechanism)
	{
		pkcs11_logger_log("  mechanism: %lu (%s)", pMechanism->mechanism, pkcs11_logger_translate_ck_mechanism_type(pMechanism->mechanism));
		pkcs11_logger_log("  pParameter: %p", pMechanism->pParameter);
		pkcs11_logger_log_byte_array("  *pParameter", pMechanism->pParameter, pMechanism->ulParameterLen);
		pkcs11_logger_log("  ulParameterLen: %p", pMechanism->ulParameterLen);
	}
	pkcs11_logger_log(" hKey: %lu", hKey);
	
	CK_RV rv = g_orig_lib_functions->C_EncryptInit(hSession, pMechanism, hKey);
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_Encrypt)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pEncryptedData, CK_ULONG_PTR pulEncryptedDataLen)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pData: %p", pData);
	pkcs11_logger_log_byte_array(" *pData", pData, ulDataLen);
	pkcs11_logger_log(" ulDataLen: %lu", ulDataLen);
	pkcs11_logger_log(" pEncryptedData: %p", pEncryptedData);
	pkcs11_logger_log(" pulEncryptedDataLen: %p", pulEncryptedDataLen);
	if (NULL != pulEncryptedDataLen)
		pkcs11_logger_log(" *pulEncryptedDataLen: %lu", *pulEncryptedDataLen);
	
	CK_RV rv = g_orig_lib_functions->C_Encrypt(hSession, pData, ulDataLen, pEncryptedData, pulEncryptedDataLen);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();
		
		pkcs11_logger_log(" pEncryptedData: %p", pEncryptedData);
		pkcs11_logger_log(" pulEncryptedDataLen: %p", pulEncryptedDataLen);
		if (NULL != pulEncryptedDataLen)
			pkcs11_logger_log_byte_array(" *pEncryptedData", pEncryptedData, *pulEncryptedDataLen);
		if (NULL != pulEncryptedDataLen)
			pkcs11_logger_log(" *pulEncryptedDataLen: %lu", *pulEncryptedDataLen);
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_EncryptUpdate)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pPart, CK_ULONG ulPartLen, CK_BYTE_PTR pEncryptedPart, CK_ULONG_PTR pulEncryptedPartLen)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pPart: %p",  pPart);
	pkcs11_logger_log_byte_array(" *pPart", pPart, ulPartLen);
	pkcs11_logger_log(" ulPartLen: %lu", ulPartLen);
	pkcs11_logger_log(" pEncryptedPart: %p",  pEncryptedPart);
	pkcs11_logger_log(" pulEncryptedPartLen: %p",  pulEncryptedPartLen);
	if (NULL != pulEncryptedPartLen)
		pkcs11_logger_log(" *pulEncryptedPartLen: %lu", *pulEncryptedPartLen);
	
	CK_RV rv = g_orig_lib_functions->C_EncryptUpdate(hSession, pPart, ulPartLen, pEncryptedPart, pulEncryptedPartLen);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();
		
		pkcs11_logger_log(" pEncryptedPart: %p", pEncryptedPart);
		pkcs11_logger_log(" pulEncryptedPartLen: %p", pulEncryptedPartLen);
		if (NULL != pulEncryptedPartLen)
			pkcs11_logger_log_byte_array(" *pEncryptedPart", pEncryptedPart, *pulEncryptedPartLen);
		if (NULL != pulEncryptedPartLen)
			pkcs11_logger_log(" *pulEncryptedPartLen: %lu", *pulEncryptedPartLen);
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_EncryptFinal)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pLastEncryptedPart, CK_ULONG_PTR pulLastEncryptedPartLen)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pLastEncryptedPart: %p", pLastEncryptedPart);
	pkcs11_logger_log(" pulLastEncryptedPartLen: %p", pulLastEncryptedPartLen);
	if (NULL != pulLastEncryptedPartLen)
		pkcs11_logger_log(" *pulLastEncryptedPartLen: %lu", *pulLastEncryptedPartLen);
	
	CK_RV rv = g_orig_lib_functions->C_EncryptFinal(hSession, pLastEncryptedPart, pulLastEncryptedPartLen);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();
		
		pkcs11_logger_log(" pLastEncryptedPart: %p", pLastEncryptedPart);
		pkcs11_logger_log(" pulLastEncryptedPartLen: %p", pulLastEncryptedPartLen);
		if (NULL != pulLastEncryptedPartLen)
			pkcs11_logger_log_byte_array(" *pLastEncryptedPart", pLastEncryptedPart, *pulLastEncryptedPartLen);
		if (NULL != pulLastEncryptedPartLen)
			pkcs11_logger_log(" *pulLastEncryptedPartLen: %lu", *pulLastEncryptedPartLen);
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_DecryptInit)(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hKey)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pMechanism: %p", pMechanism);
	if (NULL != pMechanism)
	{
		pkcs11_logger_log("  mechanism: %lu (%s)", pMechanism->mechanism, pkcs11_logger_translate_ck_mechanism_type(pMechanism->mechanism));
		pkcs11_logger_log("  pParameter: %p", pMechanism->pParameter);
		pkcs11_logger_log_byte_array(" *pParameter", pMechanism->pParameter, pMechanism->ulParameterLen);
		pkcs11_logger_log("  ulParameterLen: %lu", pMechanism->ulParameterLen);		
	}
	
	pkcs11_logger_log(" hKey: %lu", hKey);
	
	CK_RV rv = g_orig_lib_functions->C_DecryptInit(hSession, pMechanism, hKey);
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_Decrypt)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pEncryptedData, CK_ULONG ulEncryptedDataLen, CK_BYTE_PTR pData, CK_ULONG_PTR pulDataLen)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pEncryptedData: %p", pEncryptedData);
	pkcs11_logger_log_byte_array(" *pEncryptedData", pEncryptedData, ulEncryptedDataLen);
	pkcs11_logger_log(" ulEncryptedDataLen: %lu", ulEncryptedDataLen);
	pkcs11_logger_log(" pData: %p", pData);
	pkcs11_logger_log(" pulDataLen: %p", pulDataLen);
	if (NULL != pulDataLen)
		pkcs11_logger_log(" *pulDataLen: %lu", *pulDataLen);
	
	CK_RV rv = g_orig_lib_functions->C_Decrypt(hSession, pEncryptedData, ulEncryptedDataLen, pData, pulDataLen);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();
		
		pkcs11_logger_log(" pData: %p", pData);
		pkcs11_logger_log(" pulDataLen: %p", pulDataLen);
		if (NULL != pulDataLen)
			pkcs11_logger_log_byte_array(" *pData", pData, *pulDataLen);
		if (NULL != pulDataLen)
			pkcs11_logger_log(" *pulDataLen: %lu", *pulDataLen);
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_DecryptUpdate)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pEncryptedPart, CK_ULONG ulEncryptedPartLen, CK_BYTE_PTR pPart, CK_ULONG_PTR pulPartLen)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pEncryptedPart: %p",  pEncryptedPart);
	pkcs11_logger_log_byte_array(" *pEncryptedPart", pEncryptedPart, ulEncryptedPartLen);
	pkcs11_logger_log(" ulEncryptedPartLen: %lu", ulEncryptedPartLen);
	pkcs11_logger_log(" pPart: %p",  pPart);
	pkcs11_logger_log(" pulPartLen: %p",  pulPartLen);
	if (NULL != pulPartLen)
		pkcs11_logger_log(" *pulPartLen: %lu", *pulPartLen);
	
	CK_RV rv = g_orig_lib_functions->C_DecryptUpdate(hSession, pEncryptedPart, ulEncryptedPartLen, pPart, pulPartLen);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();
		
		pkcs11_logger_log(" pPart: %p", pPart);
		pkcs11_logger_log(" pulPartLen: %p", pulPartLen);
		if (NULL != pulPartLen)
			pkcs11_logger_log_byte_array(" *pPart", pPart, *pulPartLen);
		if (NULL != pulPartLen)
			pkcs11_logger_log(" *pulPartLen: %lu", *pulPartLen);
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_DecryptFinal)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pLastPart, CK_ULONG_PTR pulLastPartLen)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pLastPart: %p", pLastPart);
	pkcs11_logger_log(" pulLastPartLen: %p", pulLastPartLen);
	if (NULL != pulLastPartLen)
		pkcs11_logger_log(" *pulLastPartLen: %lu", *pulLastPartLen);
	
	CK_RV rv = g_orig_lib_functions->C_DecryptFinal(hSession, pLastPart, pulLastPartLen);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();
		
		pkcs11_logger_log(" pLastPart: %p", pLastPart);
		pkcs11_logger_log(" pulLastPartLen: %p", pulLastPartLen);
		if (NULL != pulLastPartLen)
			pkcs11_logger_log_byte_array(" *pLastPart", pLastPart, *pulLastPartLen);
		if (NULL != pulLastPartLen)
			pkcs11_logger_log(" *pulLastPartLen: %lu", *pulLastPartLen);
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_DigestInit)(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pMechanism: %p", pMechanism);
	if (NULL != pMechanism)
	{
		pkcs11_logger_log("  mechanism: %lu (%s)", pMechanism->mechanism, pkcs11_logger_translate_ck_mechanism_type(pMechanism->mechanism));
		pkcs11_logger_log("  pParameter: %p", pMechanism->pParameter);
		pkcs11_logger_log_byte_array(" *pParameter", pMechanism->pParameter, pMechanism->ulParameterLen);
		pkcs11_logger_log("  ulParameterLen: %p", pMechanism->ulParameterLen);		
	}
	
	CK_RV rv = g_orig_lib_functions->C_DigestInit(hSession, pMechanism);
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_Digest)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pDigest, CK_ULONG_PTR pulDigestLen)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pData: %p", pData);
	pkcs11_logger_log_byte_array(" *pData", pData, ulDataLen);
	pkcs11_logger_log(" ulDataLen: %lu", ulDataLen);
	pkcs11_logger_log(" pDigest: %p", pDigest);
	pkcs11_logger_log(" pulDigestLen: %p", pulDigestLen);
	if (NULL != pulDigestLen)
		pkcs11_logger_log(" *pulDigestLen: %lu", *pulDigestLen);
	
	CK_RV rv = g_orig_lib_functions->C_Digest(hSession, pData, ulDataLen, pDigest, pulDigestLen);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();
		
		pkcs11_logger_log(" pDigest: %p", pDigest);
		pkcs11_logger_log(" pulDigestLen: %p", pulDigestLen);
		if (NULL != pulDigestLen)
			pkcs11_logger_log_byte_array(" *pDigest", pDigest, *pulDigestLen);
		if (NULL != pulDigestLen)
			pkcs11_logger_log(" *pulDigestLen: %lu", *pulDigestLen);
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_DigestUpdate)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pPart, CK_ULONG ulPartLen)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pPart: %p",  pPart);
	pkcs11_logger_log_byte_array(" *pPart", pPart, ulPartLen);
	pkcs11_logger_log(" ulPartLen: %lu", ulPartLen);
	
	CK_RV rv = g_orig_lib_functions->C_DigestUpdate(hSession, pPart, ulPartLen);
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_DigestKey)(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE hKey)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" hKey: %lu", hKey);
	
	CK_RV rv = g_orig_lib_functions->C_DigestKey(hSession, hKey);
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_DigestFinal)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pDigest, CK_ULONG_PTR pulDigestLen)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pDigest: %p", pDigest);
	pkcs11_logger_log(" pulDigestLen: %p", pulDigestLen);
	if (NULL != pulDigestLen)
		pkcs11_logger_log(" *pulDigestLen: %lu", *pulDigestLen);
	
	CK_RV rv = g_orig_lib_functions->C_DigestFinal(hSession, pDigest, pulDigestLen);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();
		
		pkcs11_logger_log(" pDigest: %p", pDigest);
		pkcs11_logger_log(" pulDigestLen: %p", pulDigestLen);
		if (NULL != pulDigestLen)
			pkcs11_logger_log_byte_array(" *pDigest", pDigest, *pulDigestLen);
		if (NULL != pulDigestLen)
			pkcs11_logger_log(" *pulDigestLen: %lu", *pulDigestLen);
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_SignInit)(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hKey)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pMechanism: %p", pMechanism);
	if (NULL != pMechanism)
	{
		pkcs11_logger_log("  mechanism: %lu (%s)", pMechanism->mechanism, pkcs11_logger_translate_ck_mechanism_type(pMechanism->mechanism));
		pkcs11_logger_log("  pParameter: %p", pMechanism->pParameter);
		pkcs11_logger_log_byte_array(" *pParameter", pMechanism->pParameter, pMechanism->ulParameterLen);
		pkcs11_logger_log("  ulParameterLen: %p", pMechanism->ulParameterLen);		
	}
	pkcs11_logger_log(" hKey: %lu", hKey);
	
	CK_RV rv = g_orig_lib_functions->C_SignInit(hSession, pMechanism, hKey);
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_Sign)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pSignature, CK_ULONG_PTR pulSignatureLen)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pData: %p", pData);
	pkcs11_logger_log_byte_array(" *pData", pData, ulDataLen);
	pkcs11_logger_log(" ulDataLen: %lu", ulDataLen);
	pkcs11_logger_log(" pSignature: %p", pSignature);
	pkcs11_logger_log(" pulSignatureLen: %p", pulSignatureLen);
	if (NULL != pulSignatureLen)
		pkcs11_logger_log(" *pulSignatureLen: %lu", *pulSignatureLen);
	
	CK_RV rv = g_orig_lib_functions->C_Sign(hSession, pData, ulDataLen, pSignature, pulSignatureLen);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();
		
		pkcs11_logger_log(" pSignature: %p", pSignature);
		pkcs11_logger_log(" pulSignatureLen: %p", pulSignatureLen);
		if (NULL != pulSignatureLen)
			pkcs11_logger_log_byte_array(" *pSignature", pSignature, *pulSignatureLen);
		if (NULL != pulSignatureLen)
			pkcs11_logger_log(" *pulSignatureLen: %lu", *pulSignatureLen);
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_SignUpdate)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pPart, CK_ULONG ulPartLen)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pPart: %p", pPart);
	pkcs11_logger_log_byte_array(" *pPart", pPart, ulPartLen);
	pkcs11_logger_log(" ulPartLen: %lu", ulPartLen);
	
	CK_RV rv = g_orig_lib_functions->C_SignUpdate(hSession, pPart, ulPartLen);
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_SignFinal)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pSignature, CK_ULONG_PTR pulSignatureLen)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pSignature: %p", pSignature);
	pkcs11_logger_log(" pulSignatureLen: %p", pulSignatureLen);
	if (NULL != pulSignatureLen)
		pkcs11_logger_log(" *pulSignatureLen: %lu", *pulSignatureLen);
	
	CK_RV rv = g_orig_lib_functions->C_SignFinal(hSession, pSignature, pulSignatureLen);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();
		
		pkcs11_logger_log(" pSignature: %p", pSignature);
		pkcs11_logger_log(" pulSignatureLen: %p", pulSignatureLen);
		if (NULL != pulSignatureLen)
			pkcs11_logger_log_byte_array(" *pSignature", pSignature, *pulSignatureLen);
		if (NULL != pulSignatureLen)
			pkcs11_logger_log(" *pulSignatureLen: %lu", *pulSignatureLen);
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_SignRecoverInit)(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hKey)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pMechanism: %p", pMechanism);
	if (NULL != pMechanism)
	{
		pkcs11_logger_log("  mechanism: %lu (%s)", pMechanism->mechanism, pkcs11_logger_translate_ck_mechanism_type(pMechanism->mechanism));
		pkcs11_logger_log("  pParameter: %p", pMechanism->pParameter);
		pkcs11_logger_log_byte_array(" *pParameter", pMechanism->pParameter, pMechanism->ulParameterLen);
		pkcs11_logger_log("  ulParameterLen: %p", pMechanism->ulParameterLen);		
	}
	pkcs11_logger_log(" hKey: %lu", hKey);
	
	CK_RV rv = g_orig_lib_functions->C_SignRecoverInit(hSession, pMechanism, hKey);
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_SignRecover)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pSignature, CK_ULONG_PTR pulSignatureLen)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pData: %p", pData);
	pkcs11_logger_log_byte_array(" *pData", pData, ulDataLen);
	pkcs11_logger_log(" ulDataLen: %lu", ulDataLen);
	pkcs11_logger_log(" pSignature: %p", pSignature);
	pkcs11_logger_log(" pulSignatureLen: %p", pulSignatureLen);
	if (NULL != pulSignatureLen)
		pkcs11_logger_log(" *pulSignatureLen: %lu", *pulSignatureLen);
	
	CK_RV rv = g_orig_lib_functions->C_SignRecover(hSession, pData, ulDataLen, pSignature, pulSignatureLen);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();
		
		pkcs11_logger_log(" pSignature: %p", pSignature);
		pkcs11_logger_log(" pulSignatureLen: %p", pulSignatureLen);
		if (NULL != pulSignatureLen)
			pkcs11_logger_log_byte_array(" *pSignature", pSignature, *pulSignatureLen);
		if (NULL != pulSignatureLen)
			pkcs11_logger_log(" *pulSignatureLen: %lu", *pulSignatureLen);
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_VerifyInit)(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hKey)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pMechanism: %p", pMechanism);
	if (NULL != pMechanism)
	{
		pkcs11_logger_log("  mechanism: %lu (%s)", pMechanism->mechanism, pkcs11_logger_translate_ck_mechanism_type(pMechanism->mechanism));
		pkcs11_logger_log("  pParameter: %p", pMechanism->pParameter);
		pkcs11_logger_log_byte_array(" *pParameter", pMechanism->pParameter, pMechanism->ulParameterLen);
		pkcs11_logger_log("  ulParameterLen: %p", pMechanism->ulParameterLen);		
	}
	pkcs11_logger_log(" hKey: %lu", hKey);
	
	CK_RV rv = g_orig_lib_functions->C_VerifyInit(hSession, pMechanism, hKey);
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_Verify)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pSignature, CK_ULONG ulSignatureLen)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pData: %p", pData);
	pkcs11_logger_log_byte_array(" *pData", pData, ulDataLen);
	pkcs11_logger_log(" ulDataLen: %lu", ulDataLen);
	pkcs11_logger_log(" pSignature: %p", pSignature);
	pkcs11_logger_log_byte_array(" *pSignature", pSignature, ulSignatureLen);
	pkcs11_logger_log(" ulSignatureLen: %lu", ulSignatureLen);
	
	CK_RV rv = g_orig_lib_functions->C_Verify(hSession, pData, ulDataLen, pSignature, ulSignatureLen);
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_VerifyUpdate)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pPart, CK_ULONG ulPartLen)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pPart: %p", pPart);
	pkcs11_logger_log_byte_array(" *pPart", pPart, ulPartLen);
	pkcs11_logger_log(" ulPartLen: %lu", ulPartLen);
	
	CK_RV rv = g_orig_lib_functions->C_VerifyUpdate(hSession, pPart, ulPartLen);
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_VerifyFinal)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pSignature, CK_ULONG ulSignatureLen)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pSignature: %p", pSignature);
	pkcs11_logger_log_byte_array(" *pSignature", pSignature, ulSignatureLen);
	pkcs11_logger_log(" ulSignatureLen: %lu", ulSignatureLen);
	
	CK_RV rv = g_orig_lib_functions->C_VerifyFinal(hSession, pSignature, ulSignatureLen);
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_VerifyRecoverInit)(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hKey)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pMechanism: %p", pMechanism);
	if (NULL != pMechanism)
	{
		pkcs11_logger_log("  mechanism: %lu (%s)", pMechanism->mechanism, pkcs11_logger_translate_ck_mechanism_type(pMechanism->mechanism));
		pkcs11_logger_log("  pParameter: %p", pMechanism->pParameter);
		pkcs11_logger_log_byte_array(" *pParameter", pMechanism->pParameter, pMechanism->ulParameterLen);
		pkcs11_logger_log("  ulParameterLen: %p", pMechanism->ulParameterLen);		
	}
	pkcs11_logger_log(" hKey: %lu", hKey);
	
	CK_RV rv = g_orig_lib_functions->C_VerifyRecoverInit(hSession, pMechanism, hKey);
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_VerifyRecover)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pSignature, CK_ULONG ulSignatureLen, CK_BYTE_PTR pData, CK_ULONG_PTR pulDataLen)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pSignature: %p", pSignature);
	pkcs11_logger_log_byte_array(" *pSignature", pSignature, ulSignatureLen);
	pkcs11_logger_log(" ulSignatureLen: %lu", ulSignatureLen);
	pkcs11_logger_log(" pData: %p", pData);
	pkcs11_logger_log(" pulDataLen: %p", pulDataLen);
	if (NULL != pulDataLen)
		pkcs11_logger_log(" *pulDataLen: %lu", *pulDataLen);
	
	CK_RV rv = g_orig_lib_functions->C_VerifyRecover(hSession, pSignature, ulSignatureLen, pData, pulDataLen);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();
		
		pkcs11_logger_log(" pData: %p", pData);
		pkcs11_logger_log(" pulDataLen: %p", pulDataLen);
		if (NULL != pulDataLen)
			pkcs11_logger_log_byte_array(" *pData", pData, *pulDataLen);
		if (NULL != pulDataLen)
			pkcs11_logger_log(" *pulDataLen: %lu", *pulDataLen);
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_DigestEncryptUpdate)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pPart, CK_ULONG ulPartLen, CK_BYTE_PTR pEncryptedPart, CK_ULONG_PTR pulEncryptedPartLen)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pPart: %p",  pPart);
	pkcs11_logger_log_byte_array(" *pPart", pPart, ulPartLen);
	pkcs11_logger_log(" ulPartLen: %lu", ulPartLen);
	pkcs11_logger_log(" pEncryptedPart: %p",  pEncryptedPart);
	pkcs11_logger_log(" pulEncryptedPartLen: %p",  pulEncryptedPartLen);
	if (NULL != pulEncryptedPartLen)
		pkcs11_logger_log(" *pulEncryptedPartLen: %lu", *pulEncryptedPartLen);
	
	CK_RV rv = g_orig_lib_functions->C_DigestEncryptUpdate(hSession, pPart, ulPartLen, pEncryptedPart, pulEncryptedPartLen);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();
		
		pkcs11_logger_log(" pEncryptedPart: %p", pEncryptedPart);
		pkcs11_logger_log(" pulEncryptedPartLen: %p", pulEncryptedPartLen);
		if (NULL != pulEncryptedPartLen)
			pkcs11_logger_log_byte_array(" *pEncryptedPart", pEncryptedPart, *pulEncryptedPartLen);
		if (NULL != pulEncryptedPartLen)
			pkcs11_logger_log(" *pulEncryptedPartLen: %lu", *pulEncryptedPartLen);
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_DecryptDigestUpdate)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pEncryptedPart, CK_ULONG ulEncryptedPartLen, CK_BYTE_PTR pPart, CK_ULONG_PTR pulPartLen)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pEncryptedPart: %p",  pEncryptedPart);
	pkcs11_logger_log_byte_array(" *pEncryptedPart", pEncryptedPart, ulEncryptedPartLen);
	pkcs11_logger_log(" ulEncryptedPartLen: %lu", ulEncryptedPartLen);
	pkcs11_logger_log(" pPart: %p",  pPart);
	pkcs11_logger_log(" pulPartLen: %p",  pulPartLen);
	if (NULL != pulPartLen)
		pkcs11_logger_log(" *pulPartLen: %lu", *pulPartLen);
	
	CK_RV rv = g_orig_lib_functions->C_DecryptDigestUpdate(hSession, pEncryptedPart, ulEncryptedPartLen, pPart, pulPartLen);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();
		
		pkcs11_logger_log(" pPart: %p", pPart);
		pkcs11_logger_log(" pulPartLen: %p", pulPartLen);
		if (NULL != pulPartLen)
			pkcs11_logger_log_byte_array(" *pPart", pPart, *pulPartLen);
		if (NULL != pulPartLen)
			pkcs11_logger_log(" *pulPartLen: %lu", *pulPartLen);
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_SignEncryptUpdate)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pPart, CK_ULONG ulPartLen, CK_BYTE_PTR pEncryptedPart, CK_ULONG_PTR pulEncryptedPartLen)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pPart: %p",  pPart);
	pkcs11_logger_log_byte_array(" *pPart", pPart, ulPartLen);
	pkcs11_logger_log(" ulPartLen: %lu", ulPartLen);
	pkcs11_logger_log(" pEncryptedPart: %p",  pEncryptedPart);
	pkcs11_logger_log(" pulEncryptedPartLen: %p",  pulEncryptedPartLen);
	if (NULL != pulEncryptedPartLen)
		pkcs11_logger_log(" *pulEncryptedPartLen: %lu", *pulEncryptedPartLen);
	
	CK_RV rv = g_orig_lib_functions->C_SignEncryptUpdate(hSession, pPart, ulPartLen, pEncryptedPart, pulEncryptedPartLen);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();
		
		pkcs11_logger_log(" pEncryptedPart: %p", pEncryptedPart);
		pkcs11_logger_log(" pulEncryptedPartLen: %p", pulEncryptedPartLen);
		if (NULL != pulEncryptedPartLen)
			pkcs11_logger_log_byte_array(" *pEncryptedPart", pEncryptedPart, *pulEncryptedPartLen);
		if (NULL != pulEncryptedPartLen)
			pkcs11_logger_log(" *pulEncryptedPartLen: %lu", *pulEncryptedPartLen);
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_DecryptVerifyUpdate)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pEncryptedPart, CK_ULONG ulEncryptedPartLen, CK_BYTE_PTR pPart, CK_ULONG_PTR pulPartLen)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pEncryptedPart: %p",  pEncryptedPart);
	pkcs11_logger_log_byte_array(" *pEncryptedPart", pEncryptedPart, ulEncryptedPartLen);
	pkcs11_logger_log(" ulEncryptedPartLen: %lu", ulEncryptedPartLen);
	pkcs11_logger_log(" pPart: %p",  pPart);
	pkcs11_logger_log(" pulPartLen: %p",  pulPartLen);
	if (NULL != pulPartLen)
		pkcs11_logger_log(" *pulPartLen: %lu", *pulPartLen);
	
	CK_RV rv = g_orig_lib_functions->C_DecryptVerifyUpdate(hSession, pEncryptedPart, ulEncryptedPartLen, pPart, pulPartLen);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();
		
		pkcs11_logger_log(" pPart: %p", pPart);
		pkcs11_logger_log(" pulPartLen: %p", pulPartLen);
		if (NULL != pulPartLen)
			pkcs11_logger_log_byte_array(" *pPart", pPart, *pulPartLen);
		if (NULL != pulPartLen)
			pkcs11_logger_log(" *pulPartLen: %lu", *pulPartLen);
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_GenerateKey)(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_OBJECT_HANDLE_PTR phKey)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pMechanism: %p", pMechanism);
	if (NULL != pMechanism)
	{
		pkcs11_logger_log("  mechanism: %lu (%s)", pMechanism->mechanism, pkcs11_logger_translate_ck_mechanism_type(pMechanism->mechanism));
		pkcs11_logger_log("  pParameter: %p", pMechanism->pParameter);
		pkcs11_logger_log_byte_array("  *pParameter", pMechanism->pParameter, pMechanism->ulParameterLen);
		pkcs11_logger_log("  ulParameterLen: %p", pMechanism->ulParameterLen);		
	}
	pkcs11_logger_log(" pTemplate: %p", pTemplate);
	pkcs11_logger_log(" ulCount: %lu", ulCount);
	pkcs11_logger_log_attribute_template(pTemplate, ulCount);
	pkcs11_logger_log(" phKey: %p", phKey);
	if (NULL != phKey)
		pkcs11_logger_log(" *phKey: %lu", *phKey);
	
	CK_RV rv = g_orig_lib_functions->C_GenerateKey(hSession, pMechanism, pTemplate, ulCount, phKey);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();
		
		pkcs11_logger_log(" phKey: %p", phKey);
		if (NULL != phKey)
			pkcs11_logger_log(" *phKey: %lu", *phKey);
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_GenerateKeyPair)(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism, CK_ATTRIBUTE_PTR pPublicKeyTemplate, CK_ULONG ulPublicKeyAttributeCount, CK_ATTRIBUTE_PTR pPrivateKeyTemplate, CK_ULONG ulPrivateKeyAttributeCount, CK_OBJECT_HANDLE_PTR phPublicKey, CK_OBJECT_HANDLE_PTR phPrivateKey)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pMechanism: %p", pMechanism);
	if (NULL != pMechanism)
	{
		pkcs11_logger_log("  mechanism: %lu (%s)", pMechanism->mechanism, pkcs11_logger_translate_ck_mechanism_type(pMechanism->mechanism));
		pkcs11_logger_log("  pParameter: %p", pMechanism->pParameter);
		pkcs11_logger_log_byte_array("  *pParameter", pMechanism->pParameter, pMechanism->ulParameterLen);
		pkcs11_logger_log("  ulParameterLen: %p", pMechanism->ulParameterLen);		
	}
	pkcs11_logger_log(" pPublicKeyTemplate: %p", pPublicKeyTemplate);
	pkcs11_logger_log(" ulPublicKeyAttributeCount: %lu", ulPublicKeyAttributeCount);
	pkcs11_logger_log_attribute_template(pPublicKeyTemplate, ulPublicKeyAttributeCount);
	pkcs11_logger_log(" pPrivateKeyTemplate: %p", pPrivateKeyTemplate);
	pkcs11_logger_log(" ulPrivateKeyAttributeCount: %lu", ulPrivateKeyAttributeCount);
	pkcs11_logger_log_attribute_template(pPrivateKeyTemplate, ulPrivateKeyAttributeCount);
	pkcs11_logger_log(" phPublicKey: %p", phPublicKey);
	if (NULL != phPublicKey)
		pkcs11_logger_log(" *phPublicKey: %lu", *phPublicKey);
	pkcs11_logger_log(" phPrivateKey: %p", phPrivateKey);
	if (NULL != phPrivateKey)
		pkcs11_logger_log(" *phPrivateKey: %lu", *phPrivateKey);
	
	CK_RV rv = g_orig_lib_functions->C_GenerateKeyPair(hSession, pMechanism, pPublicKeyTemplate, ulPublicKeyAttributeCount, pPrivateKeyTemplate, ulPrivateKeyAttributeCount, phPublicKey, phPrivateKey);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();
		
		pkcs11_logger_log(" phPublicKey: %p", phPublicKey);
		if (NULL != phPublicKey)
			pkcs11_logger_log(" *phPublicKey: %lu", *phPublicKey);
		pkcs11_logger_log(" phPrivateKey: %p", phPrivateKey);
		if (NULL != phPrivateKey)
			pkcs11_logger_log(" *phPrivateKey: %lu", *phPrivateKey);
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_WrapKey)(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hWrappingKey, CK_OBJECT_HANDLE hKey, CK_BYTE_PTR pWrappedKey, CK_ULONG_PTR pulWrappedKeyLen)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pMechanism: %p", pMechanism);
	if (NULL != pMechanism)
	{
		pkcs11_logger_log("  mechanism: %lu (%s)", pMechanism->mechanism, pkcs11_logger_translate_ck_mechanism_type(pMechanism->mechanism));
		pkcs11_logger_log("  pParameter: %p", pMechanism->pParameter);
		pkcs11_logger_log_byte_array(" *pParameter", pMechanism->pParameter, pMechanism->ulParameterLen);
		pkcs11_logger_log("  ulParameterLen: %p", pMechanism->ulParameterLen);		
	}
	pkcs11_logger_log(" hWrappingKey: %lu", hWrappingKey);
	pkcs11_logger_log(" pWrappedKey: %p",  pWrappedKey);
	pkcs11_logger_log(" pulWrappedKeyLen: %p",  pulWrappedKeyLen);
	if (NULL != pulWrappedKeyLen)
		pkcs11_logger_log(" *pulWrappedKeyLen: %lu", *pulWrappedKeyLen);
	
	CK_RV rv = g_orig_lib_functions->C_WrapKey(hSession, pMechanism, hWrappingKey, hKey, pWrappedKey, pulWrappedKeyLen);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();
		
		pkcs11_logger_log(" pWrappedKey: %p", pWrappedKey);
		pkcs11_logger_log(" pulWrappedKeyLen: %p", pulWrappedKeyLen);
		if (NULL != pulWrappedKeyLen)
			pkcs11_logger_log_byte_array(" *pWrappedKey", pWrappedKey, *pulWrappedKeyLen);
		if (NULL != pulWrappedKeyLen)
			pkcs11_logger_log(" *pulWrappedKeyLen: %lu", *pulWrappedKeyLen);
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_UnwrapKey)(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hUnwrappingKey, CK_BYTE_PTR pWrappedKey, CK_ULONG ulWrappedKeyLen, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulAttributeCount, CK_OBJECT_HANDLE_PTR phKey)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pMechanism: %p", pMechanism);
	if (NULL != pMechanism)
	{
		pkcs11_logger_log("  mechanism: %lu (%s)", pMechanism->mechanism, pkcs11_logger_translate_ck_mechanism_type(pMechanism->mechanism));
		pkcs11_logger_log("  pParameter: %p", pMechanism->pParameter);
		pkcs11_logger_log_byte_array(" *pParameter", pMechanism->pParameter, pMechanism->ulParameterLen);
		pkcs11_logger_log("  ulParameterLen: %p", pMechanism->ulParameterLen);		
	}
	pkcs11_logger_log(" hUnwrappingKey: %lu", hUnwrappingKey);
	pkcs11_logger_log(" pWrappedKey: %p",  pWrappedKey);
	pkcs11_logger_log_byte_array(" *pWrappedKey", pWrappedKey, ulWrappedKeyLen);
	pkcs11_logger_log(" ulWrappedKeyLen: %lu",  ulWrappedKeyLen);
	pkcs11_logger_log(" pTemplate: %p", pTemplate);
	pkcs11_logger_log(" ulAttributeCount: %lu", ulAttributeCount);
	pkcs11_logger_log_attribute_template(pTemplate, ulAttributeCount);
	pkcs11_logger_log(" phKey: %p", phKey);
	if (NULL != phKey)
		pkcs11_logger_log(" *phKey: %lu", *phKey);
	
	CK_RV rv = g_orig_lib_functions->C_UnwrapKey(hSession, pMechanism, hUnwrappingKey, pWrappedKey, ulWrappedKeyLen, pTemplate, ulAttributeCount, phKey);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();
		
		pkcs11_logger_log(" phKey: %p", phKey);
		if (NULL != phKey)
			pkcs11_logger_log(" *phKey: %lu", *phKey);
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_DeriveKey)(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism, CK_OBJECT_HANDLE hBaseKey, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulAttributeCount, CK_OBJECT_HANDLE_PTR phKey)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pMechanism: %p", pMechanism);
	if (NULL != pMechanism)
	{
		pkcs11_logger_log("  mechanism: %lu (%s)", pMechanism->mechanism, pkcs11_logger_translate_ck_mechanism_type(pMechanism->mechanism));
		pkcs11_logger_log("  pParameter: %p", pMechanism->pParameter);
		pkcs11_logger_log_byte_array(" *pParameter", pMechanism->pParameter, pMechanism->ulParameterLen);
		pkcs11_logger_log("  ulParameterLen: %p", pMechanism->ulParameterLen);		
	}
	pkcs11_logger_log(" hBaseKey: %lu", hBaseKey);
	pkcs11_logger_log(" pTemplate: %p", pTemplate);
	pkcs11_logger_log(" ulAttributeCount: %lu", ulAttributeCount);
	pkcs11_logger_log_attribute_template(pTemplate, ulAttributeCount);
	pkcs11_logger_log(" phKey: %p", phKey);
	if (NULL != phKey)
		pkcs11_logger_log(" *phKey: %lu", *phKey);
	
	CK_RV rv = g_orig_lib_functions->C_DeriveKey(hSession, pMechanism, hBaseKey, pTemplate, ulAttributeCount, phKey);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();
		
		pkcs11_logger_log(" phKey: %p", phKey);
		if (NULL != phKey)
			pkcs11_logger_log(" *phKey: %lu", *phKey);
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_SeedRandom)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pSeed, CK_ULONG ulSeedLen)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" pSeed: %p",  pSeed);
	pkcs11_logger_log_byte_array(" *pSeed", pSeed, ulSeedLen);
	pkcs11_logger_log(" ulSeedLen: %lu",  ulSeedLen);
	
	CK_RV rv = g_orig_lib_functions->C_SeedRandom(hSession, pSeed, ulSeedLen);
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_GenerateRandom)(CK_SESSION_HANDLE hSession, CK_BYTE_PTR RandomData, CK_ULONG ulRandomLen)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	pkcs11_logger_log(" RandomData: %p",  RandomData);
	pkcs11_logger_log_byte_array(" *RandomData", RandomData, ulRandomLen);
	pkcs11_logger_log(" ulRandomLen: %lu", ulRandomLen);
	
	CK_RV rv = g_orig_lib_functions->C_GenerateRandom(hSession, RandomData, ulRandomLen);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();
		
		pkcs11_logger_log(" RandomData: %p",  RandomData);
		pkcs11_logger_log_byte_array(" *RandomData", RandomData, ulRandomLen);
		pkcs11_logger_log(" ulRandomLen: %lu", ulRandomLen);
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_GetFunctionStatus)(CK_SESSION_HANDLE hSession)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	
	CK_RV rv = g_orig_lib_functions->C_GetFunctionStatus(hSession);
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_CancelFunction)(CK_SESSION_HANDLE hSession)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" hSession: %lu", hSession);
	
	CK_RV rv = g_orig_lib_functions->C_CancelFunction(hSession);
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}


CK_DEFINE_FUNCTION(CK_RV, C_WaitForSlotEvent)(CK_FLAGS flags, CK_SLOT_ID_PTR pSlot, CK_VOID_PTR pReserved)
{
	pkcs11_logger_log_function_enter(__FUNCTION__);
	pkcs11_logger_log_input_params();
	
	pkcs11_logger_log(" flags: %lu", flags);
	pkcs11_logger_log_flag(flags, CKF_DONT_BLOCK, "  CKF_DONT_BLOCK");
	pkcs11_logger_log(" pSlot: %p", pSlot);
	pkcs11_logger_log(" pReserved: %p", pReserved);
	
	CK_RV rv = g_orig_lib_functions->C_WaitForSlotEvent(flags, pSlot, pReserved);
	
	if (CKR_OK == rv)
	{
		pkcs11_logger_log_output_params();
		
		pkcs11_logger_log(" pSlot: %p",  pSlot);
		pkcs11_logger_log(" *pSlot: %lu", pSlot);
	}
	
	pkcs11_logger_log_function_exit(rv);
	return rv;
}
