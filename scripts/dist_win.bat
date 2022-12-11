@echo off

IF "%VCPKG_ROOT%"=="" ECHO Environment variable VCPKG_ROOT not set, do you have vcpkg installed?

IF "%Qt6_DIR%"=="" ECHO Environment variable Qt6_DIR not set, do you have Qt installed?

echo "Configuring..."
cmake --preset windows-default

echo "Building..."
cmake --build --preset default

echo "Running windeployqt..."
cd bin/
%Qt6_DIR%/bin/windeployqt --release erfherder.exe
cd ..
