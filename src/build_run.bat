@echo off
cls
del .\main.exe
g++ -o main main.cpp gmath.cpp -Wall
del output0.png
if %errorlevel%==0 (
    .\main
    start output0.png
    

) else (
    echo Compilation failed.
)
echo build_run.bat complete.

pause
