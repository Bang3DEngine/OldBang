#!/bin/bash
BUILD_MODE=$1
if [ -z $BUILD_MODE ]
then
        echo "Compile mode not specified. Picking EDITOR mode (default). If you want to change it, specify as first argument: EDITOR or GAME"
        echo 
        BUILD_MODE="EDITOR"
fi

if [ "$BUILD_MODE" = "EDITOR" ]
then
	./compile.sh $BUILD_MODE && gdb -ex run ./BangEditor.exe
else
	./compile.sh $BUILD_MODE && gdb -ex run ./Game.exe
fi
