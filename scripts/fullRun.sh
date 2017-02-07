#!/bin/bash
cd "$(dirname "$0")"

BUILD_MODE="$1"
./fullCompile.sh $1 DEBUG_MODE
./run.sh $1 DEBUG_MODE
