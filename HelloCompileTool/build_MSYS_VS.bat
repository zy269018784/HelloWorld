@echo off

call "D:/application/Visual Studio 2022/Community/VC/Auxiliary/Build/vcvarsall.bat" x64

set BUILD_DIR=../../../BEngine_build/Examples/HelloCompileTool_MSYS_VS_build
set CC=cl
set CXX=cl
cmake ^
    -S . ^
    -B %BUILD_DIR% ^
    -G "MSYS Makefiles"

cmake --build %BUILD_DIR% --config Release
pause