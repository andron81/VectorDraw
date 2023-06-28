@echo off
cls
echo.
cmake -G "Visual Studio 17 2022" .. -B build_vs22_dbg -DCMAKE_BUILD_TYPE=Debug
