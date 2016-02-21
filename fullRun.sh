#!/bin/bash

./updateUI.sh

# Make and run BangPreprocessor
cd BangPreprocessor
qmake
make clean
(make && ./BangPreprocessor) || exit -1
cd ..

# Make and run Bang
qmake
make clean
(make && ./Bang) || exit -1
