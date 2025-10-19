cmake -S . -B ../../HelloQt6_build -DCMAKE_BUILD_TYPE=Release  -DUSE_QT6=true  -DCMAKE_PREFIX_PATH="F:/dev_env/PC_STM32/Qt6/6.7.3/msvc2022_64/lib/cmake"
cmake --build ../../HelloQt6_build --config Release
f:/dev_env/PC_STM32/Qt6/6.7.3/msvc2022_64/bin/windeployqt.exe --qmldir ../../HelloQt6_build/Release/ ../../HelloQt6_build/Release/HelloQt.exe



