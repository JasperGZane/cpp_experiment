@echo off
rem ============================================================
rem  Lab 2-1 build script (Windows + Visual Studio)
rem  Usage: run build.bat in this folder
rem  Output: build\task1_student_scores.exe ... (4 executables)
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

set FAILED=0
for %%f in (task1_student_scores task2_count_vowels task3_calculator task4_swap) do (
    echo [BUILD] %%f.cpp
    cl /nologo /EHsc /utf-8 /W4 /std:c++14 /Fo:build\ /Fe:build\%%f.exe %%f.cpp
    if errorlevel 1 set FAILED=1
)

if "%FAILED%"=="1" (
    echo [FAILED] Compile errors detected.
    exit /b 1
)

echo.
echo [OK] Executables generated in build\
dir /b build\*.exe
exit /b 0
