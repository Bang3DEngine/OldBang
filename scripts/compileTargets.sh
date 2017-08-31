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

RD=${RELEASE_OR_DEBUG}

CMAKE_CMD="cmake -DCMAKE_BUILD_TYPE=${RD} .."
if [ "${BANG_COMPILER}" != "" ]
then
	CMAKE_CMD="${CMAKE_CMD} -DCMAKE_CXX_COMPILER=${BANG_COMPILER}"
fi


echo "Compiling targets ${TARGETS}"
mkdir -p build${RD} && \
cd build${RD} && \
${CMAKE_CMD} && \
make ${TARGETS} \
     --no-print-directory \
     VERBOSE=${VERBOSE} \
     -j${NUM_THREADS}

EndHeader

if [[ $? != 0 ]]
then
	Error "ERROR !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
	exit 3
fi
exit 0
