@setlocal

@rem Define paths to necessary tools
set SIGNTOOL="C:\Program Files (x86)\Microsoft SDKs\ClickOnce\SignTool\signtool.exe"

@rem Define signing options
set CERTHASH=3336dc12261fed71ab08ab364bf5d0f53083b00d
set TSAURL=http://time.certum.pl/
set LIBNAME=PKCS11-LOGGER
set LIBURL=https://www.pkcs11interop.net/

@rem Sign all assemblies using SHA256withRSA algorithm
%SIGNTOOL% sign /sha1 %CERTHASH% /fd sha256 /tr %TSAURL% /td sha256 /d %LIBNAME% /du %LIBURL% ^
pkcs11-logger-x86.dll ^
pkcs11-logger-x64.dll || goto :error

@echo *** SIGN SUCCESSFUL ***
@endlocal
@exit /b 0

:error
@echo *** SIGN FAILED ***
@endlocal
@exit /b 1
