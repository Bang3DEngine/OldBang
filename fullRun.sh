#!/bin/bash

./updateUI.sh

# Make and run BangPreprocessor
cd BangPreprocessor
qmake
make clean
cd ..

# Make and run Bang
qmake
make clean

./run.sh
