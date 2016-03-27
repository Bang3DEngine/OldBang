#!/bin/bash

./updateUI.sh

# Make and run BangPreprocessor
cd BangPreprocessor
qmake
(make -j4 && ./BangPreprocessor) || exit -1
cd ..

# Make and run Bang
qmake
make -j4 && ./Bang
