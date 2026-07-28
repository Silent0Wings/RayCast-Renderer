@echo off
setlocal

set "SCRIPT_DIR=%~dp0"
set "PROJECT_ROOT=%SCRIPT_DIR%.."
set "SRC_DIR=%PROJECT_ROOT%\src"

cd /d "%SRC_DIR%" || exit /b 1

REM Define the base directory (current directory)
set "base_dir=%cd%"

REM Create folders for categories
if not exist "%base_dir%\Vec3" mkdir "%base_dir%\Vec3"
if not exist "%base_dir%\Camera" mkdir "%base_dir%\Camera"
if not exist "%base_dir%\Cube" mkdir "%base_dir%\Cube"
if not exist "%base_dir%\Suzane" mkdir "%base_dir%\Suzane"
if not exist "%base_dir%\Color" mkdir "%base_dir%\Color"
if not exist "%base_dir%\Rendering_Errors" mkdir "%base_dir%\Rendering_Errors"
if not exist "%base_dir%\Intersection_Logic" mkdir "%base_dir%\Intersection_Logic"
if not exist "%base_dir%\Space_Layout" mkdir "%base_dir%\Space_Layout"
if not exist "%base_dir%\Ray" mkdir "%base_dir%\Ray"
if not exist "%base_dir%\Miscellaneous" mkdir "%base_dir%\Miscellaneous"

REM Move files into respective folders based on keywords
move "%base_dir%\*vec3*.png" "%base_dir%\Vec3" 2>nul
move "%base_dir%\*camera*.png" "%base_dir%\Camera" 2>nul
move "%base_dir%\*cube*.png" "%base_dir%\Cube" 2>nul
move "%base_dir%\*Suzane*.png" "%base_dir%\Suzane" 2>nul
move "%base_dir%\*color*.png" "%base_dir%\Color" 2>nul
move "%base_dir%\*error*.png" "%base_dir%\Rendering_Errors" 2>nul
move "%base_dir%\*intersection*.png" "%base_dir%\Intersection_Logic" 2>nul
move "%base_dir%\*space*.png" "%base_dir%\Space_Layout" 2>nul
move "%base_dir%\*ray*.png" "%base_dir%\Ray" 2>nul

REM Handle special cases
move "%base_dir%\*Dhalia*.png" "%base_dir%\Miscellaneous" 2>nul
move "%base_dir%\*Mesh*.png" "%base_dir%\Miscellaneous" 2>nul
move "%base_dir%\*image*.png" "%base_dir%\Miscellaneous" 2>nul
move "%base_dir%\*triangle*.png" "%base_dir%\Miscellaneous" 2>nul

REM Move any remaining PNG files to Miscellaneous
move "%base_dir%\*.png" "%base_dir%\Miscellaneous" 2>nul

echo All files have been organized into respective folders.
pause
endlocal
