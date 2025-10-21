@echo off
call "D:/application/Visual Studio 2022/Community/VC/Auxiliary/Build/vcvarsall.bat" x64
set CC=cl
set CXX=cl

set BUILD_DIR=../../../BEngine_build/Examples/HelloCompileTool_MinGW_Makefiles_VS_build
cmake -S . -B %BUILD_DIR% -G "MinGW Makefiles"
cmake --build %BUILD_DIR% --config Release
