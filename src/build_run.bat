@echo off
cls

:: Check if main.exe exists and delete it
if exist .\main.exe (
    echo main.exe exists. Deleting it...
    del .\main.exe
) else (
    echo main.exe does not exist. Proceeding...
)

:: Compile the program
g++ -o main main.cpp -Wall
\\g++ -std=c++17 -o main main.cpp -Wall
:: Check if the output file (main.exe) was created successfully
if exist .\main.exe (
    :: Delete output0.png if it exists
    if exist output0.png (
        del output0.png
    )
    
    :: Run the compiled program and open output0.png
    echo Running main.exe...
    .\main
    
    if exist output0.png (
        start output0.png

    ) else (
        echo output0 does not exist.
    )


) else (
    echo Compilation failed.
)

echo build_run.bat complete.

//pause
