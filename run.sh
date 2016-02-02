#!/bin/bash

./updateUI.sh
qmake
make clean
make && ./Bang
