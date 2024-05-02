@echo off

IF "%Qt6_DIR%"=="" ECHO Environment variable Qt6_DIR not set, do you have Qt installed?

echo "Configuring..."
cmake --preset windows

echo "Building..."
cmake --build --preset default

echo "Running windeployqt..."
cd bin/
%Qt6_DIR%/bin/windeployqt --release erfherder.exe
%Qt6_DIR%/bin/windeployqt --release texview.exe
%Qt6_DIR%/bin/windeployqt --release dlg.exe
cd ..
