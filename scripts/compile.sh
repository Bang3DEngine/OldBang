#!/bin/bash

cd "$(dirname "$0")"
cd .. # Go to root directory

export QT_SELECT=5

command_exists () {
    type "$1" &> /dev/null ;
}

NUM_THREADS=16
BUILD_MODE=$1
RELEASE_OR_DEBUG=$2

echo "-----"
ERROR=0
if [ "${BUILD_MODE}" != "EDITOR" ] && [ "${BUILD_MODE}" != "GAME" ]
then
	echo "Option '${BUILD_MODE}' for the first parameter not recognised. Valid options are:"
	echo "   - 'EDITOR'"
	echo "   - 'GAME'"
	ERROR=1
fi
if [ "${RELEASE_OR_DEBUG}" != "DEBUG_MODE" ] && [ "${RELEASE_OR_DEBUG}" != "RELEASE_MODE" ]
then
	echo "Option '${RELEASE_OR_DEBUG}' for the second parameter not recognised. Valid options are:"
	echo "   - 'DEBUG_MODE'"
	echo "   - 'RELEASE_MODE'"
	ERROR=1
fi

if [ $ERROR -eq 1 ]
then
	echo ""; echo "Please fix the errors."; echo ""	
	echo "-----"
	exit 1
fi


# Make and run Bang
echo "BUILD_MODE on compile.sh:       '${BUILD_MODE}'"
echo "RELEASE_OR_DEBUG on compile.sh: '${RELEASE_OR_DEBUG}'"
echo "-----"
sleep 5

./createHeaders.sh

qmake "BUILD_MODE=${BUILD_MODE}" "RELEASE_OR_DEBUG=${RELEASE_OR_DEBUG}"
if [ $(command_exists colormake) ] && [ "${BUILD_MODE}" != "GAME" ]
then
	colormake -j${NUM_THREADS}
else
	make -j${NUM_THREADS}
fi

