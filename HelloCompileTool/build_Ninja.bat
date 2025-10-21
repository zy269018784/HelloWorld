@echo off

set BUILD_DIR=../../../BEngine_build/Examples/HelloCompileTool_Ninja_build
cmake -S . -B %BUILD_DIR% -G "Ninja"
cmake --build %BUILD_DIR% --config Release