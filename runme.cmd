cls
#rd BLD
del VectorDraw.exe
cmake -G Ninja -B  _bld . && cmake --build _bld
pause
VectorDraw.exe
