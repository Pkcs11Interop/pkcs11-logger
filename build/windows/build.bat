@rem Initialize build environment of Visual Studio 2022 Community/Professional/Enterprise
@set tools=
@set tmptools="c:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat"
@if exist %tmptools% set tools=%tmptools%
@set tmptools="c:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat"
@if exist %tmptools% set tools=%tmptools%
@set tmptools="c:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat"
@if exist %tmptools% set tools=%tmptools%
@if not defined tools goto :error

@rem Build for x86 platform
@setlocal
@echo on
call %tools% x86
@echo on
msbuild pkcs11-logger.sln /p:Configuration=Release /p:Platform=Win32 /target:Clean || goto :error
msbuild pkcs11-logger.sln /p:Configuration=Release /p:Platform=Win32 /target:Build || goto :error
copy .\Win32\Release\pkcs11-logger-x86.dll . || goto :error
@endlocal

@rem Build for x64 platform
@setlocal
@echo on
call %tools% x64
@echo on
msbuild pkcs11-logger.sln /p:Configuration=Release /p:Platform=x64 /target:Clean || goto :error
msbuild pkcs11-logger.sln /p:Configuration=Release /p:Platform=x64 /target:Build || goto :error
copy .\x64\Release\pkcs11-logger-x64.dll . || goto :error
@endlocal

@echo *** BUILD SUCCESSFUL ***
@exit /b %errorlevel%

:error
@echo *** BUILD FAILED ***
@endlocal
@exit /b %errorlevel%
