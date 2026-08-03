@echo off
setlocal EnableExtensions EnableDelayedExpansion

echo === RayCast Renderer Setup ===
echo Checking dependencies...

:: 1. Check if g++ and magick are already available on PATH
set "HAS_GXX=0"
set "HAS_MAGICK=0"

where g++ >nul 2>&1
if not errorlevel 1 set "HAS_GXX=1"

where magick >nul 2>&1
if not errorlevel 1 set "HAS_MAGICK=1"

if "%HAS_GXX%"=="1" if "%HAS_MAGICK%"=="1" (
    echo [OK] g++ and magick are both already installed and available on PATH.
    g++ --version
    magick --version
    exit /b 0
)

:: 2. Check standard MinGW / MSYS2 installation locations
set "POSSIBLE_BIN="
if exist "C:\msys64\ucrt64\bin\g++.exe" (
    set "POSSIBLE_BIN=C:\msys64\ucrt64\bin"
) else if exist "C:\mingw64\bin\g++.exe" (
    set "POSSIBLE_BIN=C:\mingw64\bin"
) else if exist "C:\Program Files\mingw-w64\bin\g++.exe" (
    set "POSSIBLE_BIN=C:\Program Files\mingw-w64\bin"
)

if defined POSSIBLE_BIN (
    echo Found tools at !POSSIBLE_BIN!
    echo Adding !POSSIBLE_BIN! to User PATH environment variable...
    powershell -NoProfile -Command "$bin='!POSSIBLE_BIN!'; $p=[Environment]::GetEnvironmentVariable('Path','User'); if(($p -split ';') -notcontains $bin){ [Environment]::SetEnvironmentVariable('Path', (($p.TrimEnd(';') + ';' + $bin).Trim(';')), 'User') }" >nul 2>&1
    set "PATH=!POSSIBLE_BIN!;%PATH%"
)

:: 3. If MSYS2 bash is present, ensure toolchain and ImageMagick are installed
if exist "C:\msys64\usr\bin\bash.exe" goto installtoolchain

:: 4. If MSYS2 is missing, attempt installation via winget
echo MSYS2 not found. Attempting to install MSYS2 via winget...
where winget >nul 2>&1
if errorlevel 1 (
    echo winget was not found. Please install MSYS2 manually.
    goto manual
)

winget install --id MSYS2.MSYS2 -e --accept-source-agreements --accept-package-agreements
if errorlevel 1 (
    echo Winget failed to install MSYS2.
    goto manual
)

echo Waiting for MSYS2 installation to complete...
set /a tries=0
:waitloop
if exist "C:\msys64\usr\bin\bash.exe" goto installtoolchain
set /a tries+=1
if !tries! GEQ 60 (
    echo Timed out waiting for MSYS2 installation.
    goto manual
)
timeout /t 2 /nobreak >nul
goto waitloop

:installtoolchain
echo Installing GCC toolchain and ImageMagick inside MSYS2...
"C:\msys64\usr\bin\bash.exe" -lc "pacman -Syu --noconfirm"
"C:\msys64\usr\bin\bash.exe" -lc "pacman -S --noconfirm --needed base-devel mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-imagemagick"
if errorlevel 1 (
    echo Package installation failed inside MSYS2.
    goto manual
)

set "MSYS2_BIN=C:\msys64\ucrt64\bin"
echo Adding !MSYS2_BIN! to User PATH...
powershell -NoProfile -Command "$bin='C:\msys64\ucrt64\bin'; $p=[Environment]::GetEnvironmentVariable('Path','User'); if(($p -split ';') -notcontains $bin){ [Environment]::SetEnvironmentVariable('Path', (($p.TrimEnd(';') + ';' + $bin).Trim(';')), 'User') }" >nul 2>&1
set "PATH=!MSYS2_BIN!;%PATH%"

where g++ >nul 2>&1
if not errorlevel 1 (
    echo [SUCCESS] Setup completed successfully!
    g++ --version
    echo NOTE: Restart any open terminals for PATH changes to apply everywhere.
    exit /b 0
)

echo Setup encountered an issue setting up PATH. Please check manual steps.

:manual
echo.
echo Manual setup steps:
echo 1. Install MSYS2 from https://www.msys2.org/
echo 2. Open the MSYS2 UCRT64 terminal and run:
echo    pacman -Syu
echo    pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-imagemagick
echo 3. Add C:\msys64\ucrt64\bin to your User PATH environment variable and reopen your terminal.
echo 4. Run this script again.
exit /b 1