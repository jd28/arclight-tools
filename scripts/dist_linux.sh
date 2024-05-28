#!/bin/bash

if [[ -z "${Qt6_DIR}" ]]; then
  echo "Environment variable Qt6_DIR not set, do you have Qt installed?"
  exit 1
fi

echo "Configuring..."
cmake --preset default

echo "Building..."
cmake --build --preset default

echo "Installing..."
cmake --install build/ --prefix .

wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
chmod a+x linuxdeploy-x86_64.AppImage

wget https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage
chmod a+x linuxdeploy-plugin-qt-x86_64.AppImage

mkdir -p AppDir/usr/bin
mv bin/arclight AppDir/usr/bin/

./linuxdeploy-x86_64.AppImage --appdir AppDir -d share/arclight.desktop -i share/arclight.png --plugin qt --output appimage
mv arclight-x86_64.AppImage bin/
