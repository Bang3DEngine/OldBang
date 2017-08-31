#!/bin/bash

cd "$(dirname "$0")"
cd .. # Go to root directory

./scripts/compileTargets.sh $1 BangPreprocessor BangMain Game.exe BangGameBuilder
