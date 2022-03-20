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
