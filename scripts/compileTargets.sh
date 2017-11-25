#!/bin/bash

cd "$(dirname "$0")"
cd .. # Go to root directory

NUM_THREADS=6

source "scripts/captureArgs.sh"

TARGETS=$*
if [ "${TARGETS}" == "" ]
then
	Error "Please specify targets to compile (following Debug/Release params)..."
	exit 2
fi

BeginHeader "COMPILING (Targets: ${TARGETS})"

VERBOSE=1
# if [ "$2" == "VERBOSE" ] ; then VERBOSE=1 ; fi

BT=${BUILD_TYPE}

CMAKE_CMD="cmake -DCMAKE_BUILD_TYPE=${BT} .."
if [ "${BANG_COMPILER}" != "" ]
then
	CMAKE_CMD="${CMAKE_CMD} -DCMAKE_CXX_COMPILER=${BANG_COMPILER}"
fi


echo "Compiling targets ${TARGETS}"
mkdir -p build${BT} && \
cd build${BT} && \
${CMAKE_CMD} && \
make ${TARGETS} \
     --no-print-directory \
     VERBOSE=${VERBOSE} \
     -j${NUM_THREADS}

if [[ $? != 0 ]]
then
	Error "ERROR !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
	EndHeader
	exit 3
fi

EndHeader
exit 0
