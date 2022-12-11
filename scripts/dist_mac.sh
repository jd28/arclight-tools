#!/bin/sh

if [[ -z "${VCPKG_ROOT}" ]]; then
  echo "Environment variable VCPKG_ROOT not set, do you have vcpkg installed?"
  exit 1
fi

if [[ -z "${Qt6_DIR}" ]]; then
  echo "Environment variable Qt6_DIR not set, do you have Qt installed?"
  exit 1
fi

echo "Configuring..."
cmake --preset macos-default

echo "Building..."
cmake --build --preset default

echo "Running macdeployqt..."
pushd bin/ > /dev/null
${Qt6_DIR}/bin/macdeployqt erfherder.app -dmg -always-overwrite
popd > /dev/null
