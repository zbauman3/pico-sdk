#!/bin/bash

if [ "$PICO_SDK_PATH" = "" ]; then
  echo "Expected \"\$PICO_SDK_PATH\" to be set."
  exit 1;
fi

if [ "$PICO_BOARD" = "" ]; then
  echo "Expected \"\$PICO_BOARD\" to be set."
  exit 1;
fi

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
  if [ "$name" = "" ]; then
    echo "Expected \"./config.json\" to contain \"name\"."
    exit 1;
  fi
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