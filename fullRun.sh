#!/bin/bash

BUILD_MODE="$1"
if [ "${BUILD_MODE}" = "EDITOR" ]
then
	./fullCompile.sh EDITOR && ./BangEditor.exe
else
	./fullCompile.sh GAME && ./Game.exe
fi
