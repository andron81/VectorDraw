@echo off
cls
echo.

set BUILD_TYPE=Debug
set BUILD_DIR=_vs19-x64

cmake -G "Visual Studio 16 2019" -A x64 .. -B %BUILD_DIR% -DCMAKE_BUILD_TYPE=%BUILD_TYPE% && ^
cmake --build %BUILD_DIR% --config %BUILD_TYPE%
