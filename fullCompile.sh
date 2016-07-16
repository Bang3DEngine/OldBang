#!/bin/bash

sleep 5

./updateUI.sh

# Make and run BangPreprocessor
cd BangPreprocessor
qmake
make clean
cd ..

# Make and run Bang
rm obj*/*

./compile.sh
