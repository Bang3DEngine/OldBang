#!/bin/bash

sleep 5

./updateUI.sh

# Make and run Bang
BUILD_MODE=$1
if [ -z "$BUILD_MODE" ]
then
	BUILD_MODE="EDITOR"
fi

if [ "$BUILD_MODE" = "EDITOR" ]
then
	rm objEditor/*
else
	rm objGame/*
fi

./compile.sh $1
