@echo off
cls
echo.
cmake -G "Visual Studio 16 2019" -A x64 .. -B _build_vs19_x64_rel -DCMAKE_BUILD_TYPE=Release
