@echo off
setlocal EnableExtensions EnableDelayedExpansion

set "SCRIPT_DIR=%~dp0"
for %%I in ("%SCRIPT_DIR%..") do set "PROJECT_ROOT=%%~fI"
set "SRC_DIR=%PROJECT_ROOT%\src"
set "BUILD_DIR=%PROJECT_ROOT%\build"
set "OUTPUT_EXE=%BUILD_DIR%\main.exe"

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

cd /d "%SRC_DIR%" || exit /b 1

:: Locate a C++ compiler
set "CXX="
if exist "C:\msys64\ucrt64\bin\g++.exe" (
    set "CXX=C:\msys64\ucrt64\bin\g++.exe"
) else if exist "C:\mingw64\bin\g++.exe" (
    set "CXX=C:\mingw64\bin\g++.exe"
) else if exist "C:\Program Files\mingw-w64\bin\g++.exe" (
    set "CXX=C:\Program Files\mingw-w64\bin\g++.exe"
) else (
    where g++ >nul 2>&1
    if not errorlevel 1 set "CXX=g++"
)

if not defined CXX (
    echo [ERROR] Could not find g++. Please run script\setup.bat first.
    exit /b 1
)

:: Add compiler bin folder to current session PATH so GCC sub-tools and runtime DLLs are found
if not "%CXX%"=="g++" (
    for %%I in ("%CXX%") do set "COMPILER_DIR=%%~dpI"
    set "PATH=!COMPILER_DIR!;%PATH%"
)

echo Using compiler: %CXX%

:: Remove previous build executable
if exist "%OUTPUT_EXE%" (
    echo Removing previous build executable: %OUTPUT_EXE%
    del /f /q "%OUTPUT_EXE%"
)

:: Clean previous rendered image files in src
if exist output0.png del /f /q output0.png
if exist output0.ppm del /f /q output0.ppm
if exist stitched.png del /f /q stitched.png
if exist stitched.ppm del /f /q stitched.ppm

:: Compile the program
echo Compiling RayCast Renderer...
"%CXX%" -std=c++17 -O0 -g -o "%OUTPUT_EXE%" main.cpp -Wall > "%BUILD_DIR%\build.log" 2>&1
set "COMPILE_EXIT=%ERRORLEVEL%"

if not "%COMPILE_EXIT%"=="0" (
    echo [ERROR] Compilation failed with exit code %COMPILE_EXIT%.
    type "%BUILD_DIR%\build.log"
    exit /b %COMPILE_EXIT%
)

echo Compilation succeeded.

:: Check if executable was created
if exist "%OUTPUT_EXE%" (
    echo Running %OUTPUT_EXE%...
    "%OUTPUT_EXE%"
    set "RUN_EXIT=!ERRORLEVEL!"
    
    if not "!RUN_EXIT!"=="0" (
        echo [ERROR] Program execution failed with exit code !RUN_EXIT!.
        exit /b !RUN_EXIT!
    )

    :: Check for generated output image files and open the available one
    if exist stitched.png (
        echo Opening stitched.png...
        start "" stitched.png
    ) else if exist stitched.ppm (
        echo Opening stitched.ppm...
        start "" stitched.ppm
    ) else if exist output0.png (
        echo Opening output0.png...
        start "" output0.png
    ) else if exist output0.ppm (
        echo Opening output0.ppm...
        start "" output0.ppm
    ) else (
        echo No rendered output image file found.
    )
) else (
    echo [ERROR] Executable was not found at %OUTPUT_EXE%.
    exit /b 1
)

echo build_run.bat complete.
endlocal
