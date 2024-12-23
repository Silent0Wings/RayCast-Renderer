@echo off

REM Define the base directory (current directory)
set "base_dir=%cd%"

REM Create folders for categories
mkdir "%base_dir%\Vec3"
mkdir "%base_dir%\Camera"
mkdir "%base_dir%\Cube"
mkdir "%base_dir%\Suzane"
mkdir "%base_dir%\Color"
mkdir "%base_dir%\Rendering_Errors"
mkdir "%base_dir%\Intersection_Logic"
mkdir "%base_dir%\Space_Layout"
mkdir "%base_dir%\Ray"
mkdir "%base_dir%\Miscellaneous"

REM Move files into respective folders based on keywords
move "%base_dir%\*vec3*.png" "%base_dir%\Vec3"
move "%base_dir%\*camera*.png" "%base_dir%\Camera"
move "%base_dir%\*cube*.png" "%base_dir%\Cube"
move "%base_dir%\*Suzane*.png" "%base_dir%\Suzane"
move "%base_dir%\*color*.png" "%base_dir%\Color"
move "%base_dir%\*error*.png" "%base_dir%\Rendering_Errors"
move "%base_dir%\*intersection*.png" "%base_dir%\Intersection_Logic"
move "%base_dir%\*space*.png" "%base_dir%\Space_Layout"
move "%base_dir%\*ray*.png" "%base_dir%\Ray"

REM Handle special cases
move "%base_dir%\*Dhalia*.png" "%base_dir%\Miscellaneous"
move "%base_dir%\*Mesh*.png" "%base_dir%\Miscellaneous"
move "%base_dir%\*image*.png" "%base_dir%\Miscellaneous"
move "%base_dir%\*triangle*.png" "%base_dir%\Miscellaneous"

REM Move any remaining PNG files to Miscellaneous
move "%base_dir%\*.png" "%base_dir%\Miscellaneous"

echo All files have been organized into respective folders.
pause
