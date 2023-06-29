@echo off
cls
echo.
cmake -G "Visual Studio 17 2022" -A x64 .. -B _build_vs22_x64_rel -DCMAKE_BUILD_TYPE=Release
