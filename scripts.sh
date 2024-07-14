#!/bin/bash

#
# Expected env vars: PICO_SDK_PATH, PICO_BOARD
#

if [ ! -f "$PWD/config.json" ]; then
  echo "Unable to find \"$PWD/config.json\""
  exit 1;
fi

# make the build directory if it doesn't exist
if [ ! -d "$PWD/build" ]; then
  mkdir -p "$PWD/build"
fi

if [ "$1" = "build-all" ]; then
  (cd ./build && cmake ../ && make);
elif [ "$1" = "upload" ]; then
  name=$(jq --raw-output ".name // empty" ./config.json);
  (cd ./build && picotool load -v "./$name.uf2" -f);
elif [ "$1" = "build" ]; then
  (cd ./build && make);
elif [ "$1" = "build-upload" ]; then
  (./scripts.sh build && ./scripts.sh upload)
elif [ "$1" = "clean" ]; then
  rm -rf ./build
else
  echo "Invalid Script";
  exit 1;
fi