@echo off
cls
setlocal enabledelayedexpansion

:: Initialize counters
set totalH=0
set totalCPP=0
set sizeH=0
set sizeCPP=0

echo Counting lines and file sizes in .h and .cpp files...

:: Loop through all .h files
for %%f in (*.h) do (
    for /f "tokens=2 delims=:" %%L in ('find /c /v "" "%%f"') do (
        set /a totalH+=%%L
        set /a sizeH+=%%~zf
        echo %%f: %%L lines, %%~zf bytes
    )
)

:: Loop through all .cpp files
for %%f in (*.cpp) do (
    for /f "tokens=2 delims=:" %%L in ('find /c /v "" "%%f"') do (
        set /a totalCPP+=%%L
        set /a sizeCPP+=%%~zf
        echo %%f: %%L lines, %%~zf bytes
    )
)

:: Print total counts
set /a totalLines=totalH+totalCPP
set /a totalSize=sizeH+sizeCPP

echo.
echo Total lines in .h files: %totalH%
echo Total size of .h files: %sizeH% bytes
echo.
echo Total lines in .cpp files: %totalCPP%
echo Total size of .cpp files: %sizeCPP% bytes
echo.
echo ++++++++++++++++++++++++++++++++++++++++++++++++++
echo Total lines in .h + .cpp files: %totalLines% 
echo ++++++++++++++++++++++++++++++++++++++++++++++++++
echo Total size of .h + .cpp files: %totalSize% bytes

pause
