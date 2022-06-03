@setlocal

@rem Initialize build environment of Visual Studio 2022
call "c:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" 

@rem Build for x86 platform
msbuild pkcs11-logger.sln /p:Configuration=Release /p:Platform=Win32 /target:Clean || goto :error
msbuild pkcs11-logger.sln /p:Configuration=Release /p:Platform=Win32 /target:Build || goto :error
copy .\Win32\Release\pkcs11-logger-x86.dll . || goto :error

@rem Build for x64 platform
msbuild pkcs11-logger.sln /p:Configuration=Release /p:Platform=x64 /target:Clean || goto :error
msbuild pkcs11-logger.sln /p:Configuration=Release /p:Platform=x64 /target:Build || goto :error
copy .\x64\Release\pkcs11-logger-x64.dll . || goto :error

@echo *** BUILD SUCCESSFUL ***
@endlocal
@exit /b %errorlevel%

:error
@echo *** BUILD FAILED ***
@endlocal
@exit /b %errorlevel%
