#!/bin/bash
cd "$(dirname "$0")"

BUILD_MODE="$1"
./fullCompile.sh $1
./run.sh $1
