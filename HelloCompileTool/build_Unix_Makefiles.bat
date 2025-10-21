@echo off

set BUILD_DIR=../../../BEngine_build/Examples/HelloCompileTool_Unix_Makefiles_build
cmake -S . -B %BUILD_DIR% -G "Unix Makefiles"
cmake --build %BUILD_DIR% --config Release