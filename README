# PKCS11-LOGGER : PKCS#11 logging proxy module

## Overview

PKCS#11 is cryptography standard published by RSA Laboratories that defines 
a platform independent API (called cryptoki) to smart cards and other types of 
cryptographic hardware.

Library implementing PKCS#11 interface is usually used in the following scenario:

	Application <--> PKCS#11 library <--> Smart card

Due to complexity of cryptoki it is not rare that user needs to troubleshoot 
communication problems between application and PKCS#11 library. That is the 
moment when PKCS11-LOGGER (logger) may come handy.

Logger sits between the application and original PKCS#11 library:

	Application <--> PKCS11-LOGGER library <--> PKCS#11 library <--> Smart card

Application calls PKCS#11 function provided by logger, logger calls the same 
function provided by original PKCS#11 library and while logging everything 
it returns the result to the application.

## Building

You will need MinGW with MSYS on Windows and GCC and Make on Linux. On both 
platforms you will need Git to get latest source with command:

	git clone https://github.com/jariq/pkcs11-logger.git

To build logger on Windows use following command:

	make -f Makefile.Windows && make

To build logger on Linux use following command:

	make -f Makefile.Linux && make

Depending on your platform there should be a library called "pkcs11-logger.dll" 
or "pkcs11-logger.so" available in your working directory.

## Usage

By default logger tries to load PKCS#11 library provided by OpenSC project 
which is "opensc-pkcs11.dll" on Windows or "opensc-pkcs11.so" on Linux. 
__PKCS11\_LOGGER\_ORIG\_LIB__ environment variable can be used to specify 
another library.

Logger records all the information into "c:\pkcs11-logger.log" on Widnows or 
"/tmp/pkcs11-logger.log" on Linux. __PKCS11\_LOGGER\_LOG\_FILE__ environment 
variable can be used to specify another log file.

## Output example

Every line starts with two hex numbers separated by a colon. First number is
process id and second one is thread id.

	0x000016ac : 0x00001020 : ****************************** 2011-11-19 21:35:48 ***
	0x000016ac : 0x00001020 : Calling C_Initialize
	0x000016ac : 0x00001020 : Input
	0x000016ac : 0x00001020 :  pInitArgs: 00000000
	0x000016ac : 0x00001020 : Returning 0 (CKR_OK)
	0x000016ac : 0x00001020 : ****************************** 2011-11-19 21:35:48 ***
	0x000016ac : 0x00001020 : Calling C_GetInfo
	0x000016ac : 0x00001020 : Input
	0x000016ac : 0x00001020 :  pInfo: 0022FECC
	0x000016ac : 0x00001020 : Output
	0x000016ac : 0x00001020 :  pInfo: 0022FECC
	0x000016ac : 0x00001020 :   cryptokiVersion:
	0x000016ac : 0x00001020 :    major: 2
	0x000016ac : 0x00001020 :    minor: 20
	0x000016ac : 0x00001020 :   manufacturerID: SoftHSM                         
	0x000016ac : 0x00001020 :   flags: 0
	0x000016ac : 0x00001020 :   libraryDescription: Implementation of PKCS11        
	0x000016ac : 0x00001020 :   libraryVersion:
	0x000016ac : 0x00001020 :    major: 1
	0x000016ac : 0x00001020 :    minor: 3
	0x000016ac : 0x00001020 : Returning 0 (CKR_OK)
