@echo off
cls

echo [1/5] Killing viewer.exe if running...
taskkill /F /IM viewer.exe >nul 2>&1

echo [2/5] Waiting for unlock...
timeout /t 1 >nul

echo [3/5] Deleting old viewer.exe...
del /f /q viewer.exe >nul 2>&1

echo [4/5] Compiling viewer.exe...
g++ -std=c++17 -mwindows main.cpp guiwindow.cpp -o viewer.exe

echo [5/5] Checking result...
if exist viewer.exe (
    echo Done. Running viewer.exe...
    start viewer.exe
) else (
    echo ERROR: Build failed. viewer.exe not found.
    pause
)
