@echo off
rem ============================================================
rem  Campus Card System build script (Windows + Visual Studio)
rem  Usage: run build.bat in this folder
rem  Output: build\CampusCardSystem.exe
rem  Note: the .sln / .vcxproj route is described in README.md
rem ============================================================
setlocal

set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" (
    echo [ERROR] vswhere.exe not found. Please install Visual Studio.
    exit /b 1
)

for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VSPATH=%%i"
if not defined VSPATH (
    echo [ERROR] Visual Studio C++ toolset not found.
    exit /b 1
)

call "%VSPATH%\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64 >nul
if errorlevel 1 (
    echo [ERROR] Failed to initialize MSVC environment.
    exit /b 1
)

if not exist build mkdir build

echo [BUILD] CampusCardSystem.cpp + card_operations.cpp
cl /nologo /EHsc /utf-8 /W4 /std:c++14 /Fo:build\ /Fe:build\CampusCardSystem.exe CampusCardSystem.cpp card_operations.cpp
if errorlevel 1 (
    echo [FAILED] Compile errors detected.
    exit /b 1
)

echo.
echo [OK] build\CampusCardSystem.exe
exit /b 0
