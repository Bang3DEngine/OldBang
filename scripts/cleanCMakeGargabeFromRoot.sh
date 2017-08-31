#!/bin/bash

cd "$(dirname "$0")"
cd .. # Go to root directory

rm -rf CMakeFiles/ cmake_install.cmake Makefile CMakeCache.txt buildDebug/ bin Bang.cbp
