call "C:\Program Files (x86)\Microsoft Visual Studio 11.0\Common7\Tools\vsvars32.bat"

msbuild pkcs11-logger.sln /p:Configuration=Release /p:Platform=Win32 /target:Clean || goto :error
msbuild pkcs11-logger.sln /p:Configuration=Release /p:Platform=Win32 /target:Build || goto :error
copy .\Win32\Release\pkcs11-logger-x86.dll . || goto :error

msbuild pkcs11-logger.sln /p:Configuration=Release /p:Platform=x64 /target:Clean || goto :error
msbuild pkcs11-logger.sln /p:Configuration=Release /p:Platform=x64 /target:Build || goto :error
copy .\x64\Release\pkcs11-logger-x64.dll . || goto :error

@echo "BUILD SUCCEEDED !!!"
@exit /b %errorlevel%

:error
@echo "BUILD FAILED !!!"
@exit /b %errorlevel%
