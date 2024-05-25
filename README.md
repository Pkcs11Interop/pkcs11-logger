PKCS11-LOGGER
=============
**PKCS#11 logging proxy module**

[![License](https://img.shields.io/badge/license-Apache%202.0-blue.svg)](https://github.com/Pkcs11Interop/pkcs11-logger/blob/master/LICENSE.md)
[![AppVeyor](https://ci.appveyor.com/api/projects/status/m4aidxggq1uo4fou/branch/master?svg=true)](https://ci.appveyor.com/project/pkcs11interop/pkcs11-logger/branch/master)
[![Stack Overflow](https://img.shields.io/badge/stack-pkcs11interop-blue.svg)](https://stackoverflow.com/questions/tagged/pkcs11interop)
[![Twitter](https://img.shields.io/badge/twitter-p11interop-blue.svg)](https://twitter.com/p11interop)

## Table of Contents

* [Overview](#overview)
* [Output example](#output-example)
* [Configuration](#configuration)
* [Download](#download)
* [Building the source](#building-the-source)
  * [Windows](#windows)
  * [Linux](#linux)
  * [macOS](#macos)
* [License](#license)
* [About](#about)

## Overview

PKCS#11 is cryptography standard that defines ANSI C API to access smart cards and other types of cryptographic hardware.

Library implementing PKCS#11 interface is usually used in the following scenario:

	Application <--> PKCS#11 library <--> Device

Due to the complexity of PKCS#11 API it is not rare that user needs to troubleshoot communication problems between application and PKCS#11 library. That is the moment when PKCS11-LOGGER (logger) may come handy.

Logger sits between the application and the original PKCS#11 library:

	Application <--> PKCS11-LOGGER library <--> PKCS#11 library <--> Device

Application calls PKCS#11 function provided by logger, logger calls the same function provided by the original PKCS#11 library and while logging everything it returns the result to the application.

## Output example

By default every logged line starts with the two hex numbers separated by a colon. The first hex number is process id and the second one is thread id.

```
0x000016ac : 0x00000000000014dc : ****************************** 2022-06-19 09:52:48 ***
0x000016ac : 0x00000000000014dc : Calling C_Initialize
0x000016ac : 0x00000000000014dc : Input
0x000016ac : 0x00000000000014dc :  pInitArgs: 000002290F5A7D10
0x000016ac : 0x00000000000014dc :   CreateMutex: 0000000000000000
0x000016ac : 0x00000000000014dc :   DestroyMutex: 0000000000000000
0x000016ac : 0x00000000000014dc :   LockMutex: 0000000000000000
0x000016ac : 0x00000000000014dc :   UnlockMutex: 0000000000000000
0x000016ac : 0x00000000000014dc :   Flags: 2
0x000016ac : 0x00000000000014dc :    CKF_LIBRARY_CANT_CREATE_OS_THREADS: FALSE
0x000016ac : 0x00000000000014dc :    CKF_OS_LOCKING_OK: TRUE
0x000016ac : 0x00000000000014dc :   pReserved: 0000000000000000
0x000016ac : 0x00000000000014dc : Returning 0 (CKR_OK)
0x000016ac : 0x00000000000014dc : ****************************** 2022-06-19 09:52:48 ***
0x000016ac : 0x00000000000014dc : Calling C_GetInfo
0x000016ac : 0x00000000000014dc : Input
0x000016ac : 0x00000000000014dc :  pInfo: 000000671F6FE040
0x000016ac : 0x00000000000014dc : Output
0x000016ac : 0x00000000000014dc :  pInfo: 000000671F6FE040
0x000016ac : 0x00000000000014dc :   cryptokiVersion:
0x000016ac : 0x00000000000014dc :    major: 2
0x000016ac : 0x00000000000014dc :    minor: 20
0x000016ac : 0x00000000000014dc :   manufacturerID: Pkcs11Interop Project           
0x000016ac : 0x00000000000014dc :   flags: 0
0x000016ac : 0x00000000000014dc :   libraryDescription: Mock module                     
0x000016ac : 0x00000000000014dc :   libraryVersion:
0x000016ac : 0x00000000000014dc :    major: 1
0x000016ac : 0x00000000000014dc :    minor: 0
0x000016ac : 0x00000000000014dc : Returning 0 (CKR_OK)
```

## Configuration

Logger behavior can be controlled with the following [environment variables](https://en.wikipedia.org/wiki/Environment_variable):

* **PKCS11_LOGGER_LIBRARY_PATH**

  Specifies the path to the original PKCS#11 library. Value needs to be provided without the enclosing quotes. When this variable is not defined all logger functions return CKR_GENERAL_ERROR and print information about missing environment variable to the stderr.

* **PKCS11_LOGGER_LOG_FILE_PATH**

  Specifies the path to the log file. Value needs to be provided without the enclosing quotes.

* **PKCS11_LOGGER_FLAGS**

  Specifies [bit mask](https://en.wikipedia.org/wiki/Mask_(computing)) that controls multiple logger features with the following meaning of individual bits:
  
  * 0x01 hex or 1 dec disables logging into the log file
  * 0x02 hex or 2 dec disables logging of process id
  * 0x04 hex or 4 dec disables logging of thread id
  * 0x08 hex or 8 dec enables logging of PINs
  * 0x10 hex or 16 dec enables logging to the stdout
  * 0x20 hex or 32 dec enables logging to the stderr
  * 0x40 hex or 64 dec enables reopening of log file (decreases performance but log file can be deleted when needed)

  Value needs to be provided as a decimal number that represents the sum of requested features. For example value 6 can be used to disable logging of process id and thread id. Default value is 0.

## Download

Signed precompiled binaries as well as source code releases can be downloaded from [releases page](https://github.com/Pkcs11Interop/pkcs11-logger/releases):

 * **Windows**  
   32-bit version: [pkcs11-logger-x86.dll](https://github.com/Pkcs11Interop/pkcs11-logger/releases/download/v2.2.0/pkcs11-logger-x86.dll)  
   64-bit version: [pkcs11-logger-x64.dll](https://github.com/Pkcs11Interop/pkcs11-logger/releases/download/v2.2.0/pkcs11-logger-x64.dll)  
   <sub>Requires [Visual C++ Redistributable for Visual Studio 2015](https://www.microsoft.com/en-us/download/details.aspx?id=48145) to be installed on the target system.</sub>
   
 * **Ubuntu 14.04**  
   32-bit version: [pkcs11-logger-x86.so](https://github.com/Pkcs11Interop/pkcs11-logger/releases/download/v2.2.0/pkcs11-logger-x86.so)  
   64-bit version: [pkcs11-logger-x64.so](https://github.com/Pkcs11Interop/pkcs11-logger/releases/download/v2.2.0/pkcs11-logger-x64.so)
   
 * **Mac OS X 10.11**  
   32-bit version: [pkcs11-logger-x86.dylib](https://github.com/Pkcs11Interop/pkcs11-logger/releases/download/v2.2.0/pkcs11-logger-x86.dylib)  
   64-bit version: [pkcs11-logger-x64.dylib](https://github.com/Pkcs11Interop/pkcs11-logger/releases/download/v2.2.0/pkcs11-logger-x64.dylib)

Archives with source code are signed with [GnuPG key of Jaroslav Imrich](https://www.jimrich.sk/crypto/).  
Windows libraries are signed with [code-signing certificate of Jaroslav Imrich](https://www.jimrich.sk/crypto/).

## Building the source

### Windows

Execute the build script on a 64-bit Windows machine with [Visual Studio 2022 Community](https://visualstudio.microsoft.com/vs/community/) (or newer) installed:

```
cd build/windows/
build.bat
```
	
The script should use Visual Studio to build both 32-bit (`pkcs11-logger-x86.dll`) and 64-bit (`pkcs11-logger-x64.dll`) versions of logger library.

### Linux

Execute the build script on a 64-bit Linux machine with GCC, GNU Make and GCC multilib support installed (available in [build-essential](https://packages.ubuntu.com/noble/build-essential) and [gcc-multilib](https://packages.ubuntu.com/noble/gcc-multilib) packages on Ubuntu 24.04 LTS):

```
cd build/linux/
sh build.sh
```

The script should use GCC to build both 32-bit (`pkcs11-logger-x86.so`) and 64-bit (`pkcs11-logger-x64.so`) versions of logger library.

### macOS

Execute the build script on a 64-bit macOS machine with [Xcode](https://developer.apple.com/xcode/) and its "Command Line Tools" extension installed:

```
cd build/macos/
sh build.sh
```

The script should use Clang to build Mach-O universal binary (`pkcs11-logger.dylib`) usable on both Apple silicon and Intel-based Mac computers.

## License

PKCS11-LOGGER is available under the terms of the [Apache License, Version 2.0](https://www.apache.org/licenses/LICENSE-2.0).  
[Human friendly license summary](https://www.tldrlegal.com/license/apache-license-2-0-apache-2-0) is available at tldrlegal.com but the [full license text](LICENSE.md) always prevails.

## About

PKCS11-LOGGER has been written for the [Pkcs11Interop](https://www.pkcs11interop.net/) project by [Jaroslav Imrich](https://www.jimrich.sk/).  
Please visit project website - [pkcs11interop.net](https://www.pkcs11interop.net) - for more information.
