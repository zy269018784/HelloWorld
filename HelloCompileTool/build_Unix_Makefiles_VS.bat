@echo off
call "C:/Disk/application/visual studio 2022/Community/VC/Auxiliary/Build/vcvarsall.bat" x64
set CC=cl
set CXX=cl

set BUILD_DIR=../../../BEngine_build/Examples/HelloCompileTool_Unix_Makefile_VS_build
cmake -S . -B %BUILD_DIR% -G  "Unix Makefiles"
cmake --build %BUILD_DIR% --config Release
