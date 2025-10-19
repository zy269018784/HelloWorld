cmake -S . -B ../../HelloQt5_build -DCMAKE_BUILD_TYPE=Release  -DUSE_QT5=true  -DCMAKE_PREFIX_PATH="F:/dev_env/PC_4060/Qt5.9.2/5.9.2/msvc2017_64/lib/cmake"
cmake --build ../../HelloQt5_build --config Release
F:/dev_env/PC_4060/Qt5.9.2/5.9.2/msvc2017_64/bin/windeployqt.exe --qmldir ../../HelloQt5_build/Release ../../HelloQt5_build/Release/HelloQt.exe


