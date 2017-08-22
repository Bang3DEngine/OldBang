#!/bin/bash

cd "$(dirname "$0")"
cd .. # Go to root directory

NUM_THREADS=6

source "scripts/captureArgs.sh"

VERBOSE=1
# if [ "$2" == "VERBOSE" ] ; then VERBOSE=1 ; fi

# CMake && Make Bang

RD=${RELEASE_OR_DEBUG}

mkdir -p build${RD} && \
cd build${RD} && \
cmake -DCMAKE_BUILD_TYPE=${RD} .. && \
make --no-print-directory \
     VERBOSE=${VERBOSE} \
     -j${NUM_THREADS}

if [[ $? != 0 ]]
then
	echo "==============================================================="
	echo "ERROR !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
	echo "==============================================================="
	exit 2
fi
exit 0
