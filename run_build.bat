#@echo off
mkdir build
cd build
cmake .. 
# cmake creates prj here but reading file outside
cmake --build . --config Release 
# builds exe here in Release folder
pause