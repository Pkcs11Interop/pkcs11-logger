/*
 *  Copyright 2011-2025 The Pkcs11Interop Project
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


// Translates CK_RV to string
const char* pkcs11_logger_translate_ck_rv(CK_RV rv)
{
    const char *rv_name = NULL;

    switch (rv)
    {
        case CKR_OK:
            rv_name = "CKR_OK";
            break;
        case CKR_CANCEL:
            rv_name = "CKR_CANCEL";
            break;
        case CKR_HOST_MEMORY:
            rv_name = "CKR_HOST_MEMORY";
            break;
        case CKR_SLOT_ID_INVALID:
            rv_name = "CKR_SLOT_ID_INVALID";
            break;
        case CKR_GENERAL_ERROR:
            rv_name = "CKR_GENERAL_ERROR";
            break;
        case CKR_FUNCTION_FAILED:
            rv_name = "CKR_FUNCTION_FAILED";
            break;
        case CKR_ARGUMENTS_BAD:
            rv_name = "CKR_ARGUMENTS_BAD";
            break;
        case CKR_NO_EVENT:
            rv_name = "CKR_NO_EVENT";
            break;
        case CKR_NEED_TO_CREATE_THREADS:
            rv_name = "CKR_NEED_TO_CREATE_THREADS";
            break;
        case CKR_CANT_LOCK:
            rv_name = "CKR_CANT_LOCK";
            break;
        case CKR_ATTRIBUTE_READ_ONLY:
            rv_name = "CKR_ATTRIBUTE_READ_ONLY";
            break;
        case CKR_ATTRIBUTE_SENSITIVE:
            rv_name = "CKR_ATTRIBUTE_SENSITIVE";
            break;
        case CKR_ATTRIBUTE_TYPE_INVALID:
            rv_name = "CKR_ATTRIBUTE_TYPE_INVALID";
            break;
        case CKR_ATTRIBUTE_VALUE_INVALID:
            rv_name = "CKR_ATTRIBUTE_VALUE_INVALID";
            break;
        case CKR_DATA_INVALID:
            rv_name = "CKR_DATA_INVALID";
            break;
        case CKR_DATA_LEN_RANGE:
            rv_name = "CKR_DATA_LEN_RANGE";
            break;
        case CKR_DEVICE_ERROR:
            rv_name = "CKR_DEVICE_ERROR";
            break;
        case CKR_DEVICE_MEMORY:
            rv_name = "CKR_DEVICE_MEMORY";
            break;
        case CKR_DEVICE_REMOVED:
            rv_name = "CKR_DEVICE_REMOVED";
            break;
        case CKR_ENCRYPTED_DATA_INVALID:
            rv_name = "CKR_ENCRYPTED_DATA_INVALID";
            break;
        case CKR_ENCRYPTED_DATA_LEN_RANGE:
            rv_name = "CKR_ENCRYPTED_DATA_LEN_RANGE";
            break;
        case CKR_FUNCTION_CANCELED:
            rv_name = "CKR_FUNCTION_CANCELED";
            break;
        case CKR_FUNCTION_NOT_PARALLEL:
            rv_name = "CKR_FUNCTION_NOT_PARALLEL";
            break;
        case CKR_FUNCTION_NOT_SUPPORTED:
            rv_name = "CKR_FUNCTION_NOT_SUPPORTED";
            break;
        case CKR_KEY_HANDLE_INVALID:
            rv_name = "CKR_KEY_HANDLE_INVALID";
            break;
        case CKR_KEY_SIZE_RANGE:
            rv_name = "CKR_KEY_SIZE_RANGE";
            break;
        case CKR_KEY_TYPE_INCONSISTENT:
            rv_name = "CKR_KEY_TYPE_INCONSISTENT";
            break;
        case CKR_KEY_NOT_NEEDED:
            rv_name = "CKR_KEY_NOT_NEEDED";
            break;
        case CKR_KEY_CHANGED:
            rv_name = "CKR_KEY_CHANGED";
            break;
        case CKR_KEY_NEEDED:
            rv_name = "CKR_KEY_NEEDED";
            break;
        case CKR_KEY_INDIGESTIBLE:
            rv_name = "CKR_KEY_INDIGESTIBLE";
            break;
        case CKR_KEY_FUNCTION_NOT_PERMITTED:
            rv_name = "CKR_KEY_FUNCTION_NOT_PERMITTED";
            break;
        case CKR_KEY_NOT_WRAPPABLE:
            rv_name = "CKR_KEY_NOT_WRAPPABLE";
            break;
        case CKR_KEY_UNEXTRACTABLE:
            rv_name = "CKR_KEY_UNEXTRACTABLE";
            break;
        case CKR_MECHANISM_INVALID:
            rv_name = "CKR_MECHANISM_INVALID";
            break;
        case CKR_MECHANISM_PARAM_INVALID:
            rv_name = "CKR_MECHANISM_PARAM_INVALID";
            break;
        case CKR_OBJECT_HANDLE_INVALID:
            rv_name = "CKR_OBJECT_HANDLE_INVALID";
            break;
        case CKR_OPERATION_ACTIVE:
            rv_name = "CKR_OPERATION_ACTIVE";
            break;
        case CKR_OPERATION_NOT_INITIALIZED:
            rv_name = "CKR_OPERATION_NOT_INITIALIZED";
            break;
        case CKR_PIN_INCORRECT:
            rv_name = "CKR_PIN_INCORRECT";
            break;
        case CKR_PIN_INVALID:
            rv_name = "CKR_PIN_INVALID";
            break;
        case CKR_PIN_LEN_RANGE:
            rv_name = "CKR_PIN_LEN_RANGE";
            break;
        case CKR_PIN_EXPIRED:
            rv_name = "CKR_PIN_EXPIRED";
            break;
        case CKR_PIN_LOCKED:
            rv_name = "CKR_PIN_LOCKED";
            break;
        case CKR_SESSION_CLOSED:
            rv_name = "CKR_SESSION_CLOSED";
            break;
        case CKR_SESSION_COUNT:
            rv_name = "CKR_SESSION_COUNT";
            break;
        case CKR_SESSION_HANDLE_INVALID:
            rv_name = "CKR_SESSION_HANDLE_INVALID";
            break;
        case CKR_SESSION_PARALLEL_NOT_SUPPORTED:
            rv_name = "CKR_SESSION_PARALLEL_NOT_SUPPORTED";
            break;
        case CKR_SESSION_READ_ONLY:
            rv_name = "CKR_SESSION_READ_ONLY";
            break;
        case CKR_SESSION_EXISTS:
            rv_name = "CKR_SESSION_EXISTS";
            break;
        case CKR_SESSION_READ_ONLY_EXISTS:
            rv_name = "CKR_SESSION_READ_ONLY_EXISTS";
            break;
        case CKR_SESSION_READ_WRITE_SO_EXISTS:
            rv_name = "CKR_SESSION_READ_WRITE_SO_EXISTS";
            break;
        case CKR_SIGNATURE_INVALID:
            rv_name = "CKR_SIGNATURE_INVALID";
            break;
        case CKR_SIGNATURE_LEN_RANGE:
            rv_name = "CKR_SIGNATURE_LEN_RANGE";
            break;
        case CKR_TEMPLATE_INCOMPLETE:
            rv_name = "CKR_TEMPLATE_INCOMPLETE";
            break;
        case CKR_TEMPLATE_INCONSISTENT:
            rv_name = "CKR_TEMPLATE_INCONSISTENT";
            break;
        case CKR_TOKEN_NOT_PRESENT:
            rv_name = "CKR_TOKEN_NOT_PRESENT";
            break;
        case CKR_TOKEN_NOT_RECOGNIZED:
            rv_name = "CKR_TOKEN_NOT_RECOGNIZED";
            break;
        case CKR_TOKEN_WRITE_PROTECTED:
            rv_name = "CKR_TOKEN_WRITE_PROTECTED";
            break;
        case CKR_UNWRAPPING_KEY_HANDLE_INVALID:
            rv_name = "CKR_UNWRAPPING_KEY_HANDLE_INVALID";
            break;
        case CKR_UNWRAPPING_KEY_SIZE_RANGE:
            rv_name = "CKR_UNWRAPPING_KEY_SIZE_RANGE";
            break;
        case CKR_UNWRAPPING_KEY_TYPE_INCONSISTENT:
            rv_name = "CKR_UNWRAPPING_KEY_TYPE_INCONSISTENT";
            break;
        case CKR_USER_ALREADY_LOGGED_IN:
            rv_name = "CKR_USER_ALREADY_LOGGED_IN";
            break;
        case CKR_USER_NOT_LOGGED_IN:
            rv_name = "CKR_USER_NOT_LOGGED_IN";
            break;
        case CKR_USER_PIN_NOT_INITIALIZED:
            rv_name = "CKR_USER_PIN_NOT_INITIALIZED";
            break;
        case CKR_USER_TYPE_INVALID:
            rv_name = "CKR_USER_TYPE_INVALID";
            break;
        case CKR_USER_ANOTHER_ALREADY_LOGGED_IN:
            rv_name = "CKR_USER_ANOTHER_ALREADY_LOGGED_IN";
            break;
        case CKR_USER_TOO_MANY_TYPES:
            rv_name = "CKR_USER_TOO_MANY_TYPES";
            break;
        case CKR_WRAPPED_KEY_INVALID:
            rv_name = "CKR_WRAPPED_KEY_INVALID";
            break;
        case CKR_WRAPPED_KEY_LEN_RANGE:
            rv_name = "CKR_WRAPPED_KEY_LEN_RANGE";
            break;
        case CKR_WRAPPING_KEY_HANDLE_INVALID:
            rv_name = "CKR_WRAPPING_KEY_HANDLE_INVALID";
            break;
        case CKR_WRAPPING_KEY_SIZE_RANGE:
            rv_name = "CKR_WRAPPING_KEY_SIZE_RANGE";
            break;
        case CKR_WRAPPING_KEY_TYPE_INCONSISTENT:
            rv_name = "CKR_WRAPPING_KEY_TYPE_INCONSISTENT";
            break;
        case CKR_RANDOM_SEED_NOT_SUPPORTED:
            rv_name = "CKR_RANDOM_SEED_NOT_SUPPORTED";
            break;
        case CKR_RANDOM_NO_RNG:
            rv_name = "CKR_RANDOM_NO_RNG";
            break;
        case CKR_DOMAIN_PARAMS_INVALID:
            rv_name = "CKR_DOMAIN_PARAMS_INVALID";
            break;
        case CKR_BUFFER_TOO_SMALL:
            rv_name = "CKR_BUFFER_TOO_SMALL";
            break;
        case CKR_SAVED_STATE_INVALID:
            rv_name = "CKR_SAVED_STATE_INVALID";
            break;
        case CKR_INFORMATION_SENSITIVE:
            rv_name = "CKR_INFORMATION_SENSITIVE";
            break;
        case CKR_STATE_UNSAVEABLE:
            rv_name = "CKR_STATE_UNSAVEABLE";
            break;
        case CKR_CRYPTOKI_NOT_INITIALIZED:
            rv_name = "CKR_CRYPTOKI_NOT_INITIALIZED";
            break;
        case CKR_CRYPTOKI_ALREADY_INITIALIZED:
            rv_name = "CKR_CRYPTOKI_ALREADY_INITIALIZED";
            break;
        case CKR_MUTEX_BAD:
            rv_name = "CKR_MUTEX_BAD";
            break;
        case CKR_MUTEX_NOT_LOCKED:
            rv_name = "CKR_MUTEX_NOT_LOCKED";
            break;
        case CKR_NEW_PIN_MODE:
            rv_name = "CKR_NEW_PIN_MODE";
            break;
        case CKR_NEXT_OTP:
            rv_name = "CKR_NEXT_OTP";
            break;
        case CKR_FUNCTION_REJECTED:
            rv_name = "CKR_FUNCTION_REJECTED";
            break;
        case CKR_VENDOR_DEFINED:
            rv_name = "CKR_VENDOR_DEFINED";
            break;
        default:
            rv_name = "Unknown";
    }

    return rv_name;
}


// Translates CK_MECHANISM_TYPE to string
const char* pkcs11_logger_translate_ck_mechanism_type(CK_MECHANISM_TYPE type)
{
    const char *type_name = NULL;

    switch (type)
    {
        case CKM_RSA_PKCS_KEY_PAIR_GEN:
            type_name = "CKM_RSA_PKCS_KEY_PAIR_GEN";
            break;
        case CKM_RSA_PKCS:
            type_name = "CKM_RSA_PKCS";
            break;
        case CKM_RSA_9796:
            type_name = "CKM_RSA_9796";
            break;
        case CKM_RSA_X_509:
            type_name = "CKM_RSA_X_509";
            break;
        case CKM_MD2_RSA_PKCS:
            type_name = "CKM_MD2_RSA_PKCS";
            break;
        case CKM_MD5_RSA_PKCS:
            type_name = "CKM_MD5_RSA_PKCS";
            break;
        case CKM_SHA1_RSA_PKCS:
            type_name = "CKM_SHA1_RSA_PKCS";
            break;
        case CKM_RIPEMD128_RSA_PKCS:
            type_name = "CKM_RIPEMD128_RSA_PKCS";
            break;
        case CKM_RIPEMD160_RSA_PKCS:
            type_name = "CKM_RIPEMD160_RSA_PKCS";
            break;
        case CKM_RSA_PKCS_OAEP:
            type_name = "CKM_RSA_PKCS_OAEP";
            break;
        case CKM_RSA_X9_31_KEY_PAIR_GEN:
            type_name = "CKM_RSA_X9_31_KEY_PAIR_GEN";
            break;
        case CKM_RSA_X9_31:
            type_name = "CKM_RSA_X9_31";
            break;
        case CKM_SHA1_RSA_X9_31:
            type_name = "CKM_SHA1_RSA_X9_31";
            break;
        case CKM_RSA_PKCS_PSS:
            type_name = "CKM_RSA_PKCS_PSS";
            break;
        case CKM_SHA1_RSA_PKCS_PSS:
            type_name = "CKM_SHA1_RSA_PKCS_PSS";
            break;
        case CKM_DSA_KEY_PAIR_GEN:
            type_name = "CKM_DSA_KEY_PAIR_GEN";
            break;
        case CKM_DSA:
            type_name = "CKM_DSA";
            break;
        case CKM_DSA_SHA1:
            type_name = "CKM_DSA_SHA1";
            break;
        case CKM_DH_PKCS_KEY_PAIR_GEN:
            type_name = "CKM_DH_PKCS_KEY_PAIR_GEN";
            break;
        case CKM_DH_PKCS_DERIVE:
            type_name = "CKM_DH_PKCS_DERIVE";
            break;
        case CKM_X9_42_DH_KEY_PAIR_GEN:
            type_name = "CKM_X9_42_DH_KEY_PAIR_GEN";
            break;
        case CKM_X9_42_DH_DERIVE:
            type_name = "CKM_X9_42_DH_DERIVE";
            break;
        case CKM_X9_42_DH_HYBRID_DERIVE:
            type_name = "CKM_X9_42_DH_HYBRID_DERIVE";
            break;
        case CKM_X9_42_MQV_DERIVE:
            type_name = "CKM_X9_42_MQV_DERIVE";
            break;
        case CKM_SHA256_RSA_PKCS:
            type_name = "CKM_SHA256_RSA_PKCS";
            break;
        case CKM_SHA384_RSA_PKCS:
            type_name = "CKM_SHA384_RSA_PKCS";
            break;
        case CKM_SHA512_RSA_PKCS:
            type_name = "CKM_SHA512_RSA_PKCS";
            break;
        case CKM_SHA256_RSA_PKCS_PSS:
            type_name = "CKM_SHA256_RSA_PKCS_PSS";
            break;
        case CKM_SHA384_RSA_PKCS_PSS:
            type_name = "CKM_SHA384_RSA_PKCS_PSS";
            break;
        case CKM_SHA512_RSA_PKCS_PSS:
            type_name = "CKM_SHA512_RSA_PKCS_PSS";
            break;
        case CKM_SHA224_RSA_PKCS:
            type_name = "CKM_SHA224_RSA_PKCS";
            break;
        case CKM_SHA224_RSA_PKCS_PSS:
            type_name = "CKM_SHA224_RSA_PKCS_PSS";
            break;
        case CKM_RC2_KEY_GEN:
            type_name = "CKM_RC2_KEY_GEN";
            break;
        case CKM_RC2_ECB:
            type_name = "CKM_RC2_ECB";
            break;
        case CKM_RC2_CBC:
            type_name = "CKM_RC2_CBC";
            break;
        case CKM_RC2_MAC:
            type_name = "CKM_RC2_MAC";
            break;
        case CKM_RC2_MAC_GENERAL:
            type_name = "CKM_RC2_MAC_GENERAL";
            break;
        case CKM_RC2_CBC_PAD:
            type_name = "CKM_RC2_CBC_PAD";
            break;
        case CKM_RC4_KEY_GEN:
            type_name = "CKM_RC4_KEY_GEN";
            break;
        case CKM_RC4:
            type_name = "CKM_RC4";
            break;
        case CKM_DES_KEY_GEN:
            type_name = "CKM_DES_KEY_GEN";
            break;
        case CKM_DES_ECB:
            type_name = "CKM_DES_ECB";
            break;
        case CKM_DES_CBC:
            type_name = "CKM_DES_CBC";
            break;
        case CKM_DES_MAC:
            type_name = "CKM_DES_MAC";
            break;
        case CKM_DES_MAC_GENERAL:
            type_name = "CKM_DES_MAC_GENERAL";
            break;
        case CKM_DES_CBC_PAD:
            type_name = "CKM_DES_CBC_PAD";
            break;
        case CKM_DES2_KEY_GEN:
            type_name = "CKM_DES2_KEY_GEN";
            break;
        case CKM_DES3_KEY_GEN:
            type_name = "CKM_DES3_KEY_GEN";
            break;
        case CKM_DES3_ECB:
            type_name = "CKM_DES3_ECB";
            break;
        case CKM_DES3_CBC:
            type_name = "CKM_DES3_CBC";
            break;
        case CKM_DES3_MAC:
            type_name = "CKM_DES3_MAC";
            break;
        case CKM_DES3_MAC_GENERAL:
            type_name = "CKM_DES3_MAC_GENERAL";
            break;
        case CKM_DES3_CBC_PAD:
            type_name = "CKM_DES3_CBC_PAD";
            break;
        case CKM_CDMF_KEY_GEN:
            type_name = "CKM_CDMF_KEY_GEN";
            break;
        case CKM_CDMF_ECB:
            type_name = "CKM_CDMF_ECB";
            break;
        case CKM_CDMF_CBC:
            type_name = "CKM_CDMF_CBC";
            break;
        case CKM_CDMF_MAC:
            type_name = "CKM_CDMF_MAC";
            break;
        case CKM_CDMF_MAC_GENERAL:
            type_name = "CKM_CDMF_MAC_GENERAL";
            break;
        case CKM_CDMF_CBC_PAD:
            type_name = "CKM_CDMF_CBC_PAD";
            break;
        case CKM_DES_OFB64:
            type_name = "CKM_DES_OFB64";
            break;
        case CKM_DES_OFB8:
            type_name = "CKM_DES_OFB8";
            break;
        case CKM_DES_CFB64:
            type_name = "CKM_DES_CFB64";
            break;
        case CKM_DES_CFB8:
            type_name = "CKM_DES_CFB8";
            break;
        case CKM_MD2:
            type_name = "CKM_MD2";
            break;
        case CKM_MD2_HMAC:
            type_name = "CKM_MD2_HMAC";
            break;
        case CKM_MD2_HMAC_GENERAL:
            type_name = "CKM_MD2_HMAC_GENERAL";
            break;
        case CKM_MD5:
            type_name = "CKM_MD5";
            break;
        case CKM_MD5_HMAC:
            type_name = "CKM_MD5_HMAC";
            break;
        case CKM_MD5_HMAC_GENERAL:
            type_name = "CKM_MD5_HMAC_GENERAL";
            break;
        case CKM_SHA_1:
            type_name = "CKM_SHA_1";
            break;
        case CKM_SHA_1_HMAC:
            type_name = "CKM_SHA_1_HMAC";
            break;
        case CKM_SHA_1_HMAC_GENERAL:
            type_name = "CKM_SHA_1_HMAC_GENERAL";
            break;
        case CKM_RIPEMD128:
            type_name = "CKM_RIPEMD128";
            break;
        case CKM_RIPEMD128_HMAC:
            type_name = "CKM_RIPEMD128_HMAC";
            break;
        case CKM_RIPEMD128_HMAC_GENERAL:
            type_name = "CKM_RIPEMD128_HMAC_GENERAL";
            break;
        case CKM_RIPEMD160:
            type_name = "CKM_RIPEMD160";
            break;
        case CKM_RIPEMD160_HMAC:
            type_name = "CKM_RIPEMD160_HMAC";
            break;
        case CKM_RIPEMD160_HMAC_GENERAL:
            type_name = "CKM_RIPEMD160_HMAC_GENERAL";
            break;
        case CKM_SHA256:
            type_name = "CKM_SHA256";
            break;
        case CKM_SHA256_HMAC:
            type_name = "CKM_SHA256_HMAC";
            break;
        case CKM_SHA256_HMAC_GENERAL:
            type_name = "CKM_SHA256_HMAC_GENERAL";
            break;
        case CKM_SHA224:
            type_name = "CKM_SHA224";
            break;
        case CKM_SHA224_HMAC:
            type_name = "CKM_SHA224_HMAC";
            break;
        case CKM_SHA224_HMAC_GENERAL:
            type_name = "CKM_SHA224_HMAC_GENERAL";
            break;
        case CKM_SHA384:
            type_name = "CKM_SHA384";
            break;
        case CKM_SHA384_HMAC:
            type_name = "CKM_SHA384_HMAC";
            break;
        case CKM_SHA384_HMAC_GENERAL:
            type_name = "CKM_SHA384_HMAC_GENERAL";
            break;
        case CKM_SHA512:
            type_name = "CKM_SHA512";
            break;
        case CKM_SHA512_HMAC:
            type_name = "CKM_SHA512_HMAC";
            break;
        case CKM_SHA512_HMAC_GENERAL:
            type_name = "CKM_SHA512_HMAC_GENERAL";
            break;
        case CKM_SECURID_KEY_GEN:
            type_name = "CKM_SECURID_KEY_GEN";
            break;
        case CKM_SECURID:
            type_name = "CKM_SECURID";
            break;
        case CKM_HOTP_KEY_GEN:
            type_name = "CKM_HOTP_KEY_GEN";
            break;
        case CKM_HOTP:
            type_name = "CKM_HOTP";
            break;
        case CKM_ACTI:
            type_name = "CKM_ACTI";
            break;
        case CKM_ACTI_KEY_GEN:
            type_name = "CKM_ACTI_KEY_GEN";
            break;
        case CKM_CAST_KEY_GEN:
            type_name = "CKM_CAST_KEY_GEN";
            break;
        case CKM_CAST_ECB:
            type_name = "CKM_CAST_ECB";
            break;
        case CKM_CAST_CBC:
            type_name = "CKM_CAST_CBC";
            break;
        case CKM_CAST_MAC:
            type_name = "CKM_CAST_MAC";
            break;
        case CKM_CAST_MAC_GENERAL:
            type_name = "CKM_CAST_MAC_GENERAL";
            break;
        case CKM_CAST_CBC_PAD:
            type_name = "CKM_CAST_CBC_PAD";
            break;
        case CKM_CAST3_KEY_GEN:
            type_name = "CKM_CAST3_KEY_GEN";
            break;
        case CKM_CAST3_ECB:
            type_name = "CKM_CAST3_ECB";
            break;
        case CKM_CAST3_CBC:
            type_name = "CKM_CAST3_CBC";
            break;
        case CKM_CAST3_MAC:
            type_name = "CKM_CAST3_MAC";
            break;
        case CKM_CAST3_MAC_GENERAL:
            type_name = "CKM_CAST3_MAC_GENERAL";
            break;
        case CKM_CAST3_CBC_PAD:
            type_name = "CKM_CAST3_CBC_PAD";
            break;
        // Duplicate values in PKCS#11 specification
        // #define CKM_CAST5_KEY_GEN              0x00000320
        // #define CKM_CAST128_KEY_GEN            0x00000320
        case CKM_CAST5_KEY_GEN:
            type_name = "CKM_CAST5_KEY_GEN or CKM_CAST128_KEY_GEN";
            break;
        // Duplicate values in PKCS#11 specification
        // #define CKM_CAST5_ECB                  0x00000321
        // #define CKM_CAST128_ECB                0x00000321
        case CKM_CAST5_ECB:
            type_name = "CKM_CAST5_ECB or CKM_CAST128_ECB";
            break;
        // Duplicate values in PKCS#11 specification
        // #define CKM_CAST5_CBC                  0x00000322
        // #define CKM_CAST128_CBC                0x00000322
        case CKM_CAST5_CBC:
            type_name = "CKM_CAST5_CBC or CKM_CAST128_CBC";
            break;
        // Duplicate values in PKCS#11 specification
        // #define CKM_CAST5_MAC                  0x00000323
        // #define CKM_CAST128_MAC                0x00000323
        case CKM_CAST5_MAC:
            type_name = "CKM_CAST5_MAC or CKM_CAST128_MAC";
            break;
        // Duplicate values in PKCS#11 specification
        // #define CKM_CAST5_MAC_GENERAL          0x00000324
        // #define CKM_CAST128_MAC_GENERAL        0x00000324
        case CKM_CAST5_MAC_GENERAL:
            type_name = "CKM_CAST5_MAC_GENERAL or CKM_CAST128_MAC_GENERAL";
            break;
        // Duplicate values in PKCS#11 specification
        // #define CKM_CAST5_CBC_PAD              0x00000325
        // #define CKM_CAST128_CBC_PAD            0x00000325
        case CKM_CAST5_CBC_PAD:
            type_name = "CKM_CAST5_CBC_PAD or CKM_CAST128_CBC_PAD";
            break;
        case CKM_RC5_KEY_GEN:
            type_name = "CKM_RC5_KEY_GEN";
            break;
        case CKM_RC5_ECB:
            type_name = "CKM_RC5_ECB";
            break;
        case CKM_RC5_CBC:
            type_name = "CKM_RC5_CBC";
            break;
        case CKM_RC5_MAC:
            type_name = "CKM_RC5_MAC";
            break;
        case CKM_RC5_MAC_GENERAL:
            type_name = "CKM_RC5_MAC_GENERAL";
            break;
        case CKM_RC5_CBC_PAD:
            type_name = "CKM_RC5_CBC_PAD";
            break;
        case CKM_IDEA_KEY_GEN:
            type_name = "CKM_IDEA_KEY_GEN";
            break;
        case CKM_IDEA_ECB:
            type_name = "CKM_IDEA_ECB";
            break;
        case CKM_IDEA_CBC:
            type_name = "CKM_IDEA_CBC";
            break;
        case CKM_IDEA_MAC:
            type_name = "CKM_IDEA_MAC";
            break;
        case CKM_IDEA_MAC_GENERAL:
            type_name = "CKM_IDEA_MAC_GENERAL";
            break;
        case CKM_IDEA_CBC_PAD:
            type_name = "CKM_IDEA_CBC_PAD";
            break;
        case CKM_GENERIC_SECRET_KEY_GEN:
            type_name = "CKM_GENERIC_SECRET_KEY_GEN";
            break;
        case CKM_CONCATENATE_BASE_AND_KEY:
            type_name = "CKM_CONCATENATE_BASE_AND_KEY";
            break;
        case CKM_CONCATENATE_BASE_AND_DATA:
            type_name = "CKM_CONCATENATE_BASE_AND_DATA";
            break;
        case CKM_CONCATENATE_DATA_AND_BASE:
            type_name = "CKM_CONCATENATE_DATA_AND_BASE";
            break;
        case CKM_XOR_BASE_AND_DATA:
            type_name = "CKM_XOR_BASE_AND_DATA";
            break;
        case CKM_EXTRACT_KEY_FROM_KEY:
            type_name = "CKM_EXTRACT_KEY_FROM_KEY";
            break;
        case CKM_SSL3_PRE_MASTER_KEY_GEN:
            type_name = "CKM_SSL3_PRE_MASTER_KEY_GEN";
            break;
        case CKM_SSL3_MASTER_KEY_DERIVE:
            type_name = "CKM_SSL3_MASTER_KEY_DERIVE";
            break;
        case CKM_SSL3_KEY_AND_MAC_DERIVE:
            type_name = "CKM_SSL3_KEY_AND_MAC_DERIVE";
            break;
        case CKM_SSL3_MASTER_KEY_DERIVE_DH:
            type_name = "CKM_SSL3_MASTER_KEY_DERIVE_DH";
            break;
        case CKM_TLS_PRE_MASTER_KEY_GEN:
            type_name = "CKM_TLS_PRE_MASTER_KEY_GEN";
            break;
        case CKM_TLS_MASTER_KEY_DERIVE:
            type_name = "CKM_TLS_MASTER_KEY_DERIVE";
            break;
        case CKM_TLS_KEY_AND_MAC_DERIVE:
            type_name = "CKM_TLS_KEY_AND_MAC_DERIVE";
            break;
        case CKM_TLS_MASTER_KEY_DERIVE_DH:
            type_name = "CKM_TLS_MASTER_KEY_DERIVE_DH";
            break;
        case CKM_TLS_PRF:
            type_name = "CKM_TLS_PRF";
            break;
        case CKM_SSL3_MD5_MAC:
            type_name = "CKM_SSL3_MD5_MAC";
            break;
        case CKM_SSL3_SHA1_MAC:
            type_name = "CKM_SSL3_SHA1_MAC";
            break;
        case CKM_MD5_KEY_DERIVATION:
            type_name = "CKM_MD5_KEY_DERIVATION";
            break;
        case CKM_MD2_KEY_DERIVATION:
            type_name = "CKM_MD2_KEY_DERIVATION";
            break;
        case CKM_SHA1_KEY_DERIVATION:
            type_name = "CKM_SHA1_KEY_DERIVATION";
            break;
        case CKM_SHA256_KEY_DERIVATION:
            type_name = "CKM_SHA256_KEY_DERIVATION";
            break;
        case CKM_SHA384_KEY_DERIVATION:
            type_name = "CKM_SHA384_KEY_DERIVATION";
            break;
        case CKM_SHA512_KEY_DERIVATION:
            type_name = "CKM_SHA512_KEY_DERIVATION";
            break;
        case CKM_SHA224_KEY_DERIVATION:
            type_name = "CKM_SHA224_KEY_DERIVATION";
            break;
        case CKM_PBE_MD2_DES_CBC:
            type_name = "CKM_PBE_MD2_DES_CBC";
            break;
        case CKM_PBE_MD5_DES_CBC:
            type_name = "CKM_PBE_MD5_DES_CBC";
            break;
        case CKM_PBE_MD5_CAST_CBC:
            type_name = "CKM_PBE_MD5_CAST_CBC";
            break;
        case CKM_PBE_MD5_CAST3_CBC:
            type_name = "CKM_PBE_MD5_CAST3_CBC";
            break;
        // Duplicate values in PKCS#11 specification
        // #define CKM_PBE_MD5_CAST5_CBC          0x000003A4
        // #define CKM_PBE_MD5_CAST128_CBC        0x000003A4
        case CKM_PBE_MD5_CAST5_CBC:
            type_name = "CKM_PBE_MD5_CAST5_CBC or CKM_PBE_MD5_CAST128_CBC";
            break;
        // Duplicate values in PKCS#11 specification
        // #define CKM_PBE_SHA1_CAST5_CBC         0x000003A5
        // #define CKM_PBE_SHA1_CAST128_CBC       0x000003A5
        case CKM_PBE_SHA1_CAST5_CBC:
            type_name = "CKM_PBE_SHA1_CAST5_CBC or CKM_PBE_SHA1_CAST128_CBC";
            break;
        case CKM_PBE_SHA1_RC4_128:
            type_name = "CKM_PBE_SHA1_RC4_128";
            break;
        case CKM_PBE_SHA1_RC4_40:
            type_name = "CKM_PBE_SHA1_RC4_40";
            break;
        case CKM_PBE_SHA1_DES3_EDE_CBC:
            type_name = "CKM_PBE_SHA1_DES3_EDE_CBC";
            break;
        case CKM_PBE_SHA1_DES2_EDE_CBC:
            type_name = "CKM_PBE_SHA1_DES2_EDE_CBC";
            break;
        case CKM_PBE_SHA1_RC2_128_CBC:
            type_name = "CKM_PBE_SHA1_RC2_128_CBC";
            break;
        case CKM_PBE_SHA1_RC2_40_CBC:
            type_name = "CKM_PBE_SHA1_RC2_40_CBC";
            break;
        case CKM_PKCS5_PBKD2:
            type_name = "CKM_PKCS5_PBKD2";
            break;
        case CKM_PBA_SHA1_WITH_SHA1_HMAC:
            type_name = "CKM_PBA_SHA1_WITH_SHA1_HMAC";
            break;
        case CKM_WTLS_PRE_MASTER_KEY_GEN:
            type_name = "CKM_WTLS_PRE_MASTER_KEY_GEN";
            break;
        case CKM_WTLS_MASTER_KEY_DERIVE:
            type_name = "CKM_WTLS_MASTER_KEY_DERIVE";
            break;
        case CKM_WTLS_MASTER_KEY_DERIVE_DH_ECC:
            type_name = "CKM_WTLS_MASTER_KEY_DERIVE_DH_ECC";
            break;
        case CKM_WTLS_PRF:
            type_name = "CKM_WTLS_PRF";
            break;
        case CKM_WTLS_SERVER_KEY_AND_MAC_DERIVE:
            type_name = "CKM_WTLS_SERVER_KEY_AND_MAC_DERIVE";
            break;
        case CKM_WTLS_CLIENT_KEY_AND_MAC_DERIVE:
            type_name = "CKM_WTLS_CLIENT_KEY_AND_MAC_DERIVE";
            break;
        case CKM_KEY_WRAP_LYNKS:
            type_name = "CKM_KEY_WRAP_LYNKS";
            break;
        case CKM_KEY_WRAP_SET_OAEP:
            type_name = "CKM_KEY_WRAP_SET_OAEP";
            break;
        case CKM_CMS_SIG:
            type_name = "CKM_CMS_SIG";
            break;
        case CKM_KIP_DERIVE:
            type_name = "CKM_KIP_DERIVE";
            break;
        case CKM_KIP_WRAP:
            type_name = "CKM_KIP_WRAP";
            break;
        case CKM_KIP_MAC:
            type_name = "CKM_KIP_MAC";
            break;
        case CKM_CAMELLIA_KEY_GEN:
            type_name = "CKM_CAMELLIA_KEY_GEN";
            break;
        case CKM_CAMELLIA_ECB:
            type_name = "CKM_CAMELLIA_ECB";
            break;
        case CKM_CAMELLIA_CBC:
            type_name = "CKM_CAMELLIA_CBC";
            break;
        case CKM_CAMELLIA_MAC:
            type_name = "CKM_CAMELLIA_MAC";
            break;
        case CKM_CAMELLIA_MAC_GENERAL:
            type_name = "CKM_CAMELLIA_MAC_GENERAL";
            break;
        case CKM_CAMELLIA_CBC_PAD:
            type_name = "CKM_CAMELLIA_CBC_PAD";
            break;
        case CKM_CAMELLIA_ECB_ENCRYPT_DATA:
            type_name = "CKM_CAMELLIA_ECB_ENCRYPT_DATA";
            break;
        case CKM_CAMELLIA_CBC_ENCRYPT_DATA:
            type_name = "CKM_CAMELLIA_CBC_ENCRYPT_DATA";
            break;
        case CKM_CAMELLIA_CTR:
            type_name = "CKM_CAMELLIA_CTR";
            break;
        case CKM_ARIA_KEY_GEN:
            type_name = "CKM_ARIA_KEY_GEN";
            break;
        case CKM_ARIA_ECB:
            type_name = "CKM_ARIA_ECB";
            break;
        case CKM_ARIA_CBC:
            type_name = "CKM_ARIA_CBC";
            break;
        case CKM_ARIA_MAC:
            type_name = "CKM_ARIA_MAC";
            break;
        case CKM_ARIA_MAC_GENERAL:
            type_name = "CKM_ARIA_MAC_GENERAL";
            break;
        case CKM_ARIA_CBC_PAD:
            type_name = "CKM_ARIA_CBC_PAD";
            break;
        case CKM_ARIA_ECB_ENCRYPT_DATA:
            type_name = "CKM_ARIA_ECB_ENCRYPT_DATA";
            break;
        case CKM_ARIA_CBC_ENCRYPT_DATA:
            type_name = "CKM_ARIA_CBC_ENCRYPT_DATA";
            break;
        case CKM_SKIPJACK_KEY_GEN:
            type_name = "CKM_SKIPJACK_KEY_GEN";
            break;
        case CKM_SKIPJACK_ECB64:
            type_name = "CKM_SKIPJACK_ECB64";
            break;
        case CKM_SKIPJACK_CBC64:
            type_name = "CKM_SKIPJACK_CBC64";
            break;
        case CKM_SKIPJACK_OFB64:
            type_name = "CKM_SKIPJACK_OFB64";
            break;
        case CKM_SKIPJACK_CFB64:
            type_name = "CKM_SKIPJACK_CFB64";
            break;
        case CKM_SKIPJACK_CFB32:
            type_name = "CKM_SKIPJACK_CFB32";
            break;
        case CKM_SKIPJACK_CFB16:
            type_name = "CKM_SKIPJACK_CFB16";
            break;
        case CKM_SKIPJACK_CFB8:
            type_name = "CKM_SKIPJACK_CFB8";
            break;
        case CKM_SKIPJACK_WRAP:
            type_name = "CKM_SKIPJACK_WRAP";
            break;
        case CKM_SKIPJACK_PRIVATE_WRAP:
            type_name = "CKM_SKIPJACK_PRIVATE_WRAP";
            break;
        case CKM_SKIPJACK_RELAYX:
            type_name = "CKM_SKIPJACK_RELAYX";
            break;
        case CKM_KEA_KEY_PAIR_GEN:
            type_name = "CKM_KEA_KEY_PAIR_GEN";
            break;
        case CKM_KEA_KEY_DERIVE:
            type_name = "CKM_KEA_KEY_DERIVE";
            break;
        case CKM_FORTEZZA_TIMESTAMP:
            type_name = "CKM_FORTEZZA_TIMESTAMP";
            break;
        case CKM_BATON_KEY_GEN:
            type_name = "CKM_BATON_KEY_GEN";
            break;
        case CKM_BATON_ECB128:
            type_name = "CKM_BATON_ECB128";
            break;
        case CKM_BATON_ECB96:
            type_name = "CKM_BATON_ECB96";
            break;
        case CKM_BATON_CBC128:
            type_name = "CKM_BATON_CBC128";
            break;
        case CKM_BATON_COUNTER:
            type_name = "CKM_BATON_COUNTER";
            break;
        case CKM_BATON_SHUFFLE:
            type_name = "CKM_BATON_SHUFFLE";
            break;
        case CKM_BATON_WRAP:
            type_name = "CKM_BATON_WRAP";
            break;
        // Duplicate values in PKCS#11 specification
        // #define CKM_ECDSA_KEY_PAIR_GEN         0x00001040
        // #define CKM_EC_KEY_PAIR_GEN            0x00001040
        case CKM_ECDSA_KEY_PAIR_GEN:
            type_name = "CKM_ECDSA_KEY_PAIR_GEN or CKM_EC_KEY_PAIR_GEN";
            break;
        case CKM_ECDSA:
            type_name = "CKM_ECDSA";
            break;
        case CKM_ECDSA_SHA1:
            type_name = "CKM_ECDSA_SHA1";
            break;
        case CKM_ECDH1_DERIVE:
            type_name = "CKM_ECDH1_DERIVE";
            break;
        case CKM_ECDH1_COFACTOR_DERIVE:
            type_name = "CKM_ECDH1_COFACTOR_DERIVE";
            break;
        case CKM_ECMQV_DERIVE:
            type_name = "CKM_ECMQV_DERIVE";
            break;
        case CKM_JUNIPER_KEY_GEN:
            type_name = "CKM_JUNIPER_KEY_GEN";
            break;
        case CKM_JUNIPER_ECB128:
            type_name = "CKM_JUNIPER_ECB128";
            break;
        case CKM_JUNIPER_CBC128:
            type_name = "CKM_JUNIPER_CBC128";
            break;
        case CKM_JUNIPER_COUNTER:
            type_name = "CKM_JUNIPER_COUNTER";
            break;
        case CKM_JUNIPER_SHUFFLE:
            type_name = "CKM_JUNIPER_SHUFFLE";
            break;
        case CKM_JUNIPER_WRAP:
            type_name = "CKM_JUNIPER_WRAP";
            break;
        case CKM_FASTHASH:
            type_name = "CKM_FASTHASH";
            break;
        case CKM_AES_KEY_GEN:
            type_name = "CKM_AES_KEY_GEN";
            break;
        case CKM_AES_ECB:
            type_name = "CKM_AES_ECB";
            break;
        case CKM_AES_CBC:
            type_name = "CKM_AES_CBC";
            break;
        case CKM_AES_MAC:
            type_name = "CKM_AES_MAC";
            break;
        case CKM_AES_MAC_GENERAL:
            type_name = "CKM_AES_MAC_GENERAL";
            break;
        case CKM_AES_CBC_PAD:
            type_name = "CKM_AES_CBC_PAD";
            break;
        case CKM_AES_CTR:
            type_name = "CKM_AES_CTR";
            break;
        case CKM_BLOWFISH_KEY_GEN:
            type_name = "CKM_BLOWFISH_KEY_GEN";
            break;
        case CKM_BLOWFISH_CBC:
            type_name = "CKM_BLOWFISH_CBC";
            break;
        case CKM_TWOFISH_KEY_GEN:
            type_name = "CKM_TWOFISH_KEY_GEN";
            break;
        case CKM_TWOFISH_CBC:
            type_name = "CKM_TWOFISH_CBC";
            break;
        case CKM_DES_ECB_ENCRYPT_DATA:
            type_name = "CKM_DES_ECB_ENCRYPT_DATA";
            break;
        case CKM_DES_CBC_ENCRYPT_DATA:
            type_name = "CKM_DES_CBC_ENCRYPT_DATA";
            break;
        case CKM_DES3_ECB_ENCRYPT_DATA:
            type_name = "CKM_DES3_ECB_ENCRYPT_DATA";
            break;
        case CKM_DES3_CBC_ENCRYPT_DATA:
            type_name = "CKM_DES3_CBC_ENCRYPT_DATA";
            break;
        case CKM_AES_ECB_ENCRYPT_DATA:
            type_name = "CKM_AES_ECB_ENCRYPT_DATA";
            break;
        case CKM_AES_CBC_ENCRYPT_DATA:
            type_name = "CKM_AES_CBC_ENCRYPT_DATA";
            break;
        case CKM_DSA_PARAMETER_GEN:
            type_name = "CKM_DSA_PARAMETER_GEN";
            break;
        case CKM_DH_PKCS_PARAMETER_GEN:
            type_name = "CKM_DH_PKCS_PARAMETER_GEN";
            break;
        case CKM_X9_42_DH_PARAMETER_GEN:
            type_name = "CKM_X9_42_DH_PARAMETER_GEN";
            break;
        case CKM_VENDOR_DEFINED:
            type_name = "CKM_VENDOR_DEFINED";
            break;
        default:
            type_name = "Unknown";
    }
    
    return type_name;
}


// Translates CK_USER_TYPE to string
const char* pkcs11_logger_translate_ck_user_type(CK_USER_TYPE type)
{
    const char *type_name = NULL;

    switch (type)
    {
        case CKU_SO:
            type_name = "CKU_SO";
            break;
        case CKU_USER:
            type_name = "CKU_USER";
            break;
        case CKU_CONTEXT_SPECIFIC:
            type_name = "CKU_CONTEXT_SPECIFIC";
            break;
        default:
            type_name = "Unknown";
    }
    
    return type_name;
}


// Translates CK_STATE to string
const char* pkcs11_logger_translate_ck_state(CK_STATE state)
{
    const char *state_name = NULL;

    switch (state)
    {
        case CKS_RO_PUBLIC_SESSION:
            state_name = "CKS_RO_PUBLIC_SESSION";
            break;
        case CKS_RO_USER_FUNCTIONS:
            state_name = "CKS_RO_USER_FUNCTIONS";
            break;
        case CKS_RW_PUBLIC_SESSION:
            state_name = "CKS_RW_PUBLIC_SESSION";
            break;
        case CKS_RW_USER_FUNCTIONS:
            state_name = "CKS_RW_USER_FUNCTIONS";
            break;
        case CKS_RW_SO_FUNCTIONS:
            state_name = "CKS_RW_SO_FUNCTIONS";
            break;
        default:
            state_name = "Unknown";
    }
    
    return state_name;
}


// Translates CK_BYTE_PTR to string
char* pkcs11_logger_translate_ck_byte_ptr(CK_BYTE_PTR bytes, CK_ULONG length)
{
    CK_ULONG i = 0;
    char *output = NULL;
    CK_ULONG output_len = (length * 2) + 1;
    const char *t = "0123456789ABCDEF";
    
    output = (char *) malloc(output_len);
    if (NULL == output)
        return NULL;

    memset(output, 0, output_len);

    for (i = 0; i < length; i++)
    {
        int lo = bytes[i] & 0x0F;
        int hi = bytes[i] >> 4;
        output[i * 2] = t[hi];
        output[i * 2 + 1] = t[lo];
    }

    return output;
}


// Translates CK_ATTRIBUTE_TYPE to string
const char* pkcs11_logger_translate_ck_attribute(CK_ATTRIBUTE_TYPE type)
{
    const char *type_name = NULL;

    switch (type)
    {
        case CKA_CLASS:
            type_name = "CKA_CLASS";
            break;
        case CKA_TOKEN:
            type_name = "CKA_TOKEN";
            break;
        case CKA_PRIVATE:
            type_name = "CKA_PRIVATE";
            break;
        case CKA_LABEL:
            type_name = "CKA_LABEL";
            break;
        case CKA_APPLICATION:
            type_name = "CKA_APPLICATION";
            break;
        case CKA_VALUE:
            type_name = "CKA_VALUE";
            break;
        case CKA_OBJECT_ID:
            type_name = "CKA_OBJECT_ID";
            break;
        case CKA_CERTIFICATE_TYPE:
            type_name = "CKA_CERTIFICATE_TYPE";
            break;
        case CKA_ISSUER:
            type_name = "CKA_ISSUER";
            break;
        case CKA_SERIAL_NUMBER:
            type_name = "CKA_SERIAL_NUMBER";
            break;
        case CKA_AC_ISSUER:
            type_name = "CKA_AC_ISSUER";
            break;
        case CKA_OWNER:
            type_name = "CKA_OWNER";
            break;
        case CKA_ATTR_TYPES:
            type_name = "CKA_ATTR_TYPES";
            break;
        case CKA_TRUSTED:
            type_name = "CKA_TRUSTED";
            break;
        case CKA_CERTIFICATE_CATEGORY:
            type_name = "CKA_CERTIFICATE_CATEGORY";
            break;
        case CKA_JAVA_MIDP_SECURITY_DOMAIN:
            type_name = "CKA_JAVA_MIDP_SECURITY_DOMAIN";
            break;
        case CKA_URL:
            type_name = "CKA_URL";
            break;
        case CKA_HASH_OF_SUBJECT_PUBLIC_KEY:
            type_name = "CKA_HASH_OF_SUBJECT_PUBLIC_KEY";
            break;
        case CKA_HASH_OF_ISSUER_PUBLIC_KEY:
            type_name = "CKA_HASH_OF_ISSUER_PUBLIC_KEY";
            break;
        case CKA_CHECK_VALUE:
            type_name = "CKA_CHECK_VALUE";
            break;
        case CKA_KEY_TYPE:
            type_name = "CKA_KEY_TYPE";
            break;
        case CKA_SUBJECT:
            type_name = "CKA_SUBJECT";
            break;
        case CKA_ID:
            type_name = "CKA_ID";
            break;
        case CKA_SENSITIVE:
            type_name = "CKA_SENSITIVE";
            break;
        case CKA_ENCRYPT:
            type_name = "CKA_ENCRYPT";
            break;
        case CKA_DECRYPT:
            type_name = "CKA_DECRYPT";
            break;
        case CKA_WRAP:
            type_name = "CKA_WRAP";
            break;
        case CKA_UNWRAP:
            type_name = "CKA_UNWRAP";
            break;
        case CKA_SIGN:
            type_name = "CKA_SIGN";
            break;
        case CKA_SIGN_RECOVER:
            type_name = "CKA_SIGN_RECOVER";
            break;
        case CKA_VERIFY:
            type_name = "CKA_VERIFY";
            break;
        case CKA_VERIFY_RECOVER:
            type_name = "CKA_VERIFY_RECOVER";
            break;
        case CKA_DERIVE:
            type_name = "CKA_DERIVE";
            break;
        case CKA_START_DATE:
            type_name = "CKA_START_DATE";
            break;
        case CKA_END_DATE:
            type_name = "CKA_END_DATE";
            break;
        case CKA_MODULUS:
            type_name = "CKA_MODULUS";
            break;
        case CKA_MODULUS_BITS:
            type_name = "CKA_MODULUS_BITS";
            break;
        case CKA_PUBLIC_EXPONENT:
            type_name = "CKA_PUBLIC_EXPONENT";
            break;
        case CKA_PRIVATE_EXPONENT:
            type_name = "CKA_PRIVATE_EXPONENT";
            break;
        case CKA_PRIME_1:
            type_name = "CKA_PRIME_1";
            break;
        case CKA_PRIME_2:
            type_name = "CKA_PRIME_2";
            break;
        case CKA_EXPONENT_1:
            type_name = "CKA_EXPONENT_1";
            break;
        case CKA_EXPONENT_2:
            type_name = "CKA_EXPONENT_2";
            break;
        case CKA_COEFFICIENT:
            type_name = "CKA_COEFFICIENT";
            break;
        case CKA_PRIME:
            type_name = "CKA_PRIME";
            break;
        case CKA_SUBPRIME:
            type_name = "CKA_SUBPRIME";
            break;
        case CKA_BASE:
            type_name = "CKA_BASE";
            break;
        case CKA_PRIME_BITS:
            type_name = "CKA_PRIME_BITS";
            break;
        // Duplicate values in PKCS#11 specification
        // #define CKA_SUBPRIME_BITS      0x00000134
        // #define CKA_SUB_PRIME_BITS     CKA_SUBPRIME_BITS
        case CKA_SUBPRIME_BITS:
            type_name = "CKA_SUBPRIME_BITS or CKA_SUB_PRIME_BITS";
            break;
        case CKA_VALUE_BITS:
            type_name = "CKA_VALUE_BITS";
            break;
        case CKA_VALUE_LEN:
            type_name = "CKA_VALUE_LEN";
            break;
        case CKA_EXTRACTABLE:
            type_name = "CKA_EXTRACTABLE";
            break;
        case CKA_LOCAL:
            type_name = "CKA_LOCAL";
            break;
        case CKA_NEVER_EXTRACTABLE:
            type_name = "CKA_NEVER_EXTRACTABLE";
            break;
        case CKA_ALWAYS_SENSITIVE:
            type_name = "CKA_ALWAYS_SENSITIVE";
            break;
        case CKA_KEY_GEN_MECHANISM:
            type_name = "CKA_KEY_GEN_MECHANISM";
            break;
        case CKA_MODIFIABLE:
            type_name = "CKA_MODIFIABLE";
            break;
        // Duplicate values in PKCS#11 specification    
        // #define CKA_ECDSA_PARAMS       0x00000180
        // #define CKA_EC_PARAMS          0x00000180
        case CKA_EC_PARAMS:
            type_name = "CKA_EC_PARAMS or CKA_ECDSA_PARAMS";
            break;
        case CKA_EC_POINT:
            type_name = "CKA_EC_POINT";
            break;
        case CKA_SECONDARY_AUTH:
            type_name = "CKA_SECONDARY_AUTH";
            break;
        case CKA_AUTH_PIN_FLAGS:
            type_name = "CKA_AUTH_PIN_FLAGS";
            break;
        case CKA_ALWAYS_AUTHENTICATE:
            type_name = "CKA_ALWAYS_AUTHENTICATE";
            break;
        case CKA_WRAP_WITH_TRUSTED:
            type_name = "CKA_WRAP_WITH_TRUSTED";
            break;
        case CKA_WRAP_TEMPLATE:
            type_name = "CKA_WRAP_TEMPLATE";
            break;
        case CKA_UNWRAP_TEMPLATE:
            type_name = "CKA_UNWRAP_TEMPLATE";
            break;
        case CKA_OTP_FORMAT:
            type_name = "CKA_OTP_FORMAT";
            break;
        case CKA_OTP_LENGTH:
            type_name = "CKA_OTP_LENGTH";
            break;
        case CKA_OTP_TIME_INTERVAL:
            type_name = "CKA_OTP_TIME_INTERVAL";
            break;
        case CKA_OTP_USER_FRIENDLY_MODE:
            type_name = "CKA_OTP_USER_FRIENDLY_MODE";
            break;
        case CKA_OTP_CHALLENGE_REQUIREMENT:
            type_name = "CKA_OTP_CHALLENGE_REQUIREMENT";
            break;
        case CKA_OTP_TIME_REQUIREMENT:
            type_name = "CKA_OTP_TIME_REQUIREMENT";
            break;
        case CKA_OTP_COUNTER_REQUIREMENT:
            type_name = "CKA_OTP_COUNTER_REQUIREMENT";
            break;
        case CKA_OTP_PIN_REQUIREMENT:
            type_name = "CKA_OTP_PIN_REQUIREMENT";
            break;
        case CKA_OTP_COUNTER:
            type_name = "CKA_OTP_COUNTER";
            break;
        case CKA_OTP_TIME:
            type_name = "CKA_OTP_TIME";
            break;
        case CKA_OTP_USER_IDENTIFIER:
            type_name = "CKA_OTP_USER_IDENTIFIER";
            break;
        case CKA_OTP_SERVICE_IDENTIFIER:
            type_name = "CKA_OTP_SERVICE_IDENTIFIER";
            break;
        case CKA_OTP_SERVICE_LOGO:
            type_name = "CKA_OTP_SERVICE_LOGO";
            break;
        case CKA_OTP_SERVICE_LOGO_TYPE:
            type_name = "CKA_OTP_SERVICE_LOGO_TYPE";
            break;
        case CKA_HW_FEATURE_TYPE:
            type_name = "CKA_HW_FEATURE_TYPE";
            break;
        case CKA_RESET_ON_INIT:
            type_name = "CKA_RESET_ON_INIT";
            break;
        case CKA_HAS_RESET:
            type_name = "CKA_HAS_RESET";
            break;
        case CKA_PIXEL_X:
            type_name = "CKA_PIXEL_X";
            break;
        case CKA_PIXEL_Y:
            type_name = "CKA_PIXEL_Y";
            break;
        case CKA_RESOLUTION:
            type_name = "CKA_RESOLUTION";
            break;
        case CKA_CHAR_ROWS:
            type_name = "CKA_CHAR_ROWS";
            break;
        case CKA_CHAR_COLUMNS:
            type_name = "CKA_CHAR_COLUMNS";
            break;
        case CKA_COLOR:
            type_name = "CKA_COLOR";
            break;
        case CKA_BITS_PER_PIXEL:
            type_name = "CKA_BITS_PER_PIXEL";
            break;
        case CKA_CHAR_SETS:
            type_name = "CKA_CHAR_SETS";
            break;
        case CKA_ENCODING_METHODS:
            type_name = "CKA_ENCODING_METHODS";
            break;
        case CKA_MIME_TYPES:
            type_name = "CKA_MIME_TYPES";
            break;
        case CKA_MECHANISM_TYPE:
            type_name = "CKA_MECHANISM_TYPE";
            break;
        case CKA_REQUIRED_CMS_ATTRIBUTES:
            type_name = "CKA_REQUIRED_CMS_ATTRIBUTES";
            break;
        case CKA_DEFAULT_CMS_ATTRIBUTES:
            type_name = "CKA_DEFAULT_CMS_ATTRIBUTES";
            break;
        case CKA_SUPPORTED_CMS_ATTRIBUTES:
            type_name = "CKA_SUPPORTED_CMS_ATTRIBUTES";
            break;
        case CKA_ALLOWED_MECHANISMS:
            type_name = "CKA_ALLOWED_MECHANISMS";
            break;
        case CKA_VENDOR_DEFINED:
            type_name = "CKA_VENDOR_DEFINED";
            break;
        default:
            type_name = "Unknown";
    }
    
    return type_name;
}
