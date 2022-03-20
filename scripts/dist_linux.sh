#!/bin/bash

if [[ -z "${VCPKG_ROOT}" ]]; then
  echo "Environment variable VCPKG_ROOT not set, do you have vcpkg installed?"
  exit 1
fi

if [[ -z "${Qt5_DIR}" ]]; then
  echo "Environment variable Qt5_DIR not set, do you have Qt installed?"
  exit 1
fi

echo "Configuring..."
cmake --preset default

echo "Building..."
cmake --build --preset default

LINUXDEPLOYQT=$(pwd)/linuxdeployqt-continuous-x86_64.AppImage
echo "Running linuxdeployqt..."
pushd bin/ > /dev/null
PATH=${Qt5_DIR}/bin:${PATH} ${LINUXDEPLOYQT} bin/erfherder -bundle-non-qt-libs ${LINUXDEPLOYQT_OPTS}
PATH=${Qt5_DIR}/bin:${PATH} ${LINUXDEPLOYQT} bin/erfherder -appimage ${LINUXDEPLOYQT_OPTS}
popd > /dev/null
