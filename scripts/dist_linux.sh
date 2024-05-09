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
