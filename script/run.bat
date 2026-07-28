@echo off
setlocal

set "SCRIPT_DIR=%~dp0"
set "PROJECT_ROOT=%SCRIPT_DIR%.."
set "SRC_DIR=%PROJECT_ROOT%\src"

cd /d "%SRC_DIR%" || exit /b 1

if exist main.exe (
    echo Running main.exe...
    main.exe
) else (
    echo main.exe not found. Build it first with build_run.bat
    pause
)

endlocal
