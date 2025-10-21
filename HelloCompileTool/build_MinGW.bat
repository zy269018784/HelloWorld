@echo off

set BUILD_DIR=../../../BEngine_build/Examples/HelloCompileTool_MinGW_Makefiles_build
cmake -S . -B %BUILD_DIR% -G "MinGW Makefiles"
cmake --build %BUILD_DIR% --config Release