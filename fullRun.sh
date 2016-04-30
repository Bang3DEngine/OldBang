#!/bin/bash

./updateUI.sh

# Make and run BangPreprocessor
echo ; echo ; echo ;
echo "---------------------------------"
echo "MAKE AND RUN OF BANGPREPROCESSOR"
echo "---------------------------------"
cd BangPreprocessor
qmake
make clean
(make -j8 && ./BangPreprocessor) || exit -1
cd ..

echo ; echo "---------------------------------"; echo

# Make and run Bang
echo ; echo ; echo
echo "-----------------------------"
echo "MAKE AND RUN OF BANG"
echo "-----------------------------"
qmake
make clean
(make -j8 && ./Bang) || exit -1
