#!/usr/bin/env bash
set -e

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${ROOT_DIR}/build"
BUILD_TYPE="${BUILD_TYPE:-Release}"

mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"

case "$(uname -s)" in
  Linux*)
    GENERATOR="Unix Makefiles"
    ;;
  Darwin*)
    GENERATOR="Unix Makefiles"
    ;;
  MINGW*|MSYS*|CYGWIN*)
    GENERATOR="Ninja"
    ;;
  *)
    GENERATOR="Unix Makefiles"
    ;;
esac

if ! command -v cmake >/dev/null 2>&1; then
  echo "Error: cmake not found on PATH."
  exit 1
fi

echo "Configuring project in ${BUILD_DIR} using generator: ${GENERATOR}"
cmake -G "${GENERATOR}" -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" "${ROOT_DIR}"

echo "Building project"
if [[ "${GENERATOR}" == *"Visual Studio"* ]]; then
  cmake --build . --config "${BUILD_TYPE}"
else
  cmake --build .
fi
