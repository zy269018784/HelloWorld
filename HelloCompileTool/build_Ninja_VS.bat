@echo off
call "C:/Disk/application/visual studio 2022/Community/VC/Auxiliary/Build/vcvarsall.bat" x64
set BUILD_DIR="../../../BEngine_build/Examples/HelloCompileTool_Ninja_VS_build"
set CC=cl
set CXX=cl
cmake -S . -B %BUILD_DIR% -G  "Ninja"
cmake --build %BUILD_DIR% --config Release
pause