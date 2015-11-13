PKCS11-LOGGER
=============
**PKCS#11 logging proxy module**

## Table of Contents

* [Overview](#overview)
* [Output example](#output-example)
* [Configuration](#configuration)
* [Download](#download)
* [Building the source](#building-the-source)
  * [Windows](#windows)
  * [Linux](#linux)
  * [Mac OS X](#mac-os-x)
* [License](#license)
* [About](#about)

## Overview

PKCS#11 is cryptography standard originally published by RSA Laboratories that 
defines ANSI C API (called cryptoki) to access smart cards and other types 
of cryptographic hardware. Standard is currently being maintained and developed 
by the OASIS PKCS 11 Technical Committee.

Library implementing PKCS#11 interface is usually used in the following scenario:

	Application <--> PKCS#11 library <--> Device

Due to the complexity of cryptoki it is not rare that user needs to troubleshoot 
communication problems between application and PKCS#11 library. That is the 
moment when PKCS11-LOGGER (logger) may come handy.

Logger sits between the application and the original PKCS#11 library:

	Application <--> PKCS11-LOGGER library <--> PKCS#11 library <--> Device

Application calls PKCS#11 function provided by logger, logger calls the same 
function provided by the original PKCS#11 library and while logging everything 
it returns the result to the application.

## Output example

By default every logged line starts with the two hex numbers separated by 
a colon. The first hex number is process id and the second one is thread id.

	0x000016ac : 0x00001020 : ****************************** 2014-02-10 22:42:01 ***
	0x000016ac : 0x00001020 : Calling C_Initialize
	0x000016ac : 0x00001020 : Input
	0x000016ac : 0x00001020 :  pInitArgs: 00000000
	0x000016ac : 0x00001020 : Returning 0 (CKR_OK)
	0x000016ac : 0x00001020 : ****************************** 2014-02-10 22:42:01 ***
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

## Configuration

Logger behavior can be controlled with the following [environment variables]
(http://en.wikipedia.org/wiki/Environment_variable):

* **PKCS11_LOGGER_LIBRARY_PATH**

  Specifies the path to the original PKCS#11 library. Value needs to be provided 
  without the enclosing quotes. When this variable is not defined all logger 
  functions return CKR_GENERAL_ERROR and print information about missing 
  environment variable to the stderr.

* **PKCS11_LOGGER_LOG_FILE_PATH**

  Specifies the path to the log file. Value needs to be provided without the 
  enclosing quotes.

* **PKCS11_LOGGER_FLAGS**

  Specifies [bit mask](http://en.wikipedia.org/wiki/Mask_(computing)) that 
  controls multiple logger features with the following meaning of individual 
  bits:
  
  * 0x01 hex or 1 dec disables logging into the log file
  * 0x02 hex or 2 dec disables logging of process id
  * 0x04 hex or 4 dec disables logging of thread id
  * 0x08 hex or 8 dec enables logging of PINs
  * 0x10 hex or 16 dec enables logging to the stdout
  * 0x20 hex or 32 dec enables logging to the stderr
  * 0x40 hex or 64 dec enables reopening of log file (decreases performance but
    log file can be deleted when needed)

  Value needs to be provided as a decimal number that represents the sum of 
  requested features. For example value 6 can be used to disable logging of 
  process id and thread id. Default value is 0.

## Download

Signed precompiled binaries as well as source code releases can be downloaded from 
[sourceforge.net](http://sourceforge.net/projects/pkcs11-logger/):

 * **Windows**  
   32-bit version: [pkcs11-logger-x86.dll](http://sourceforge.net/projects/pkcs11-logger/files/2.0.0/windows/pkcs11-logger-x86.dll/download)  
   64-bit version: [pkcs11-logger-x64.dll](http://sourceforge.net/projects/pkcs11-logger/files/2.0.0/windows/pkcs11-logger-x64.dll/download)  
   <sub>Requires [Visual C++ Redistributable for Visual Studio 2012]
(http://www.microsoft.com/en-us/download/details.aspx?id=30679) 
to be installed on the target system.</sub>

 * **Ubuntu 12.04**  
   32-bit version: [pkcs11-logger-x86.so](http://sourceforge.net/projects/pkcs11-logger/files/2.0.0/linux/pkcs11-logger-x86.so/download)  
   64-bit version: [pkcs11-logger-x64.so](http://sourceforge.net/projects/pkcs11-logger/files/2.0.0/linux/pkcs11-logger-x64.so/download)
   
 * **Mac OS X**  
   32-bit version: [pkcs11-logger-x86.dylib](http://sourceforge.net/projects/pkcs11-logger/files/2.0.0/osx/pkcs11-logger-x86.dylib/download)  
   64-bit version: [pkcs11-logger-x64.dylib](http://sourceforge.net/projects/pkcs11-logger/files/2.0.0/osx/pkcs11-logger-x64.dylib/download)

Archives with source code are signed with [GnuPG key of Jaroslav Imrich]
(https://www.jimrich.sk/crypto/).  
Windows libraries are signed with [code-signing certificate of Jaroslav Imrich]
(https://www.jimrich.sk/crypto/).

## Building the source

### Windows

Execute the build script on a 64-bit Windows machine with [Visual Studio 
Express 2012 for Windows Desktop](http://www.microsoft.com/en-us/download/details.aspx?id=34673) 
(or higher) installed:

	cd build/windows/
	build.bat
	
The script should use Visual Studio to build both 32-bit (pkcs11-logger-x86.dll) 
and 64-bit (pkcs11-logger-x64.dll) versions of logger library.

### Linux

Execute the build script on a 64-bit Linux machine with GCC, GNU Make and GCC 
multilib support installed (available in [build-essential](http://packages.ubuntu.com/precise/build-essential) 
and [gcc-multilib](http://packages.ubuntu.com/precise/gcc-multilib) packages on Ubuntu 12.04 LTS):

	cd build/linux/
	sh build.sh

The script should use GCC to build both 32-bit (pkcs11-logger-x86.so) 
and 64-bit (pkcs11-logger-x64.so) versions of logger library.

### Mac OS X

Execute the build script on a 64-bit Mac OS X machine with [Xcode](https://developer.apple.com/xcode/) 
and its "Command Line Tools" extension installed:

	cd build/osx/
	sh build.sh

The script should use GCC to build both 32-bit (pkcs11-logger-x86.dylib) 
and 64-bit (pkcs11-logger-x64.dylib) versions of logger library.

## License

PKCS11-LOGGER uses dual-licensing model:

* **Licensing for open source projects:** PKCS11-LOGGER is available under the 
terms of the [GNU Affero General Public License version 3]
(http://www.gnu.org/licenses/agpl-3.0.html) as published by the Free Software 
Foundation.  
* **Licensing for other types of projects:** PKCS11-LOGGER is available under 
the terms of flexible commercial license. Please contact JWC s.r.o. at 
[info@pkcs11interop.net](mailto:info@pkcs11interop.net) for more details.

## About

PKCS11-LOGGER has been developed as a part of [Pkcs11Interop]
(http://www.pkcs11interop.net/) project by [Jaroslav Imrich](http://www.jimrich.sk/).  
Commercial license and support are provided by Slovakia (EU) based company 
[JWC s.r.o.](http://www.jwc.sk/)
