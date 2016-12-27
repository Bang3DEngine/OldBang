#!/bin/bash

BUILD_MODE="$1"
./fullCompile.sh $1

BUILD_MODE="$1"
if [ -z "$BUILD_MODE" ]
then
	BUILD_MODE="EDITOR"
fi

if [ "${BUILD_MODE}" = "EDITOR" ]
then
	./BangEditor.exe
else
	./Game.exe
fi
