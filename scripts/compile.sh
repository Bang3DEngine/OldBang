#!/bin/bash

cd "$(dirname "$0")"
cd .. # Go to root directory

export QT_SELECT=5

NUM_THREADS=6
EDITOR_OR_GAME=$1
RELEASE_OR_DEBUG=$2

echo "-----"

ERROR=0
if [ "${EDITOR_OR_GAME}" != "EDITOR" ] && [ "${EDITOR_OR_GAME}" != "GAME" ]
then
	echo "Option '${EDITOR_OR_GAME}' for the first parameter not recognised. Valid options are:"
	echo "   - 'EDITOR'"
	echo "   - 'GAME'"
	ERROR=1
fi

if [ "${RELEASE_OR_DEBUG}" != "DEBUG" ] && [ "${RELEASE_OR_DEBUG}" != "RELEASE" ]
then
	echo "Option '${RELEASE_OR_DEBUG}' for the second parameter not recognised. Valid options are:"
	echo "   - 'DEBUG'"
	echo "   - 'RELEASE'"
	ERROR=1
fi

if [ $ERROR -eq 1 ]
then
	echo ""; echo "Please fix the errors."; echo ""	
	echo "-----"
	exit 1
fi


# Make and run Bang
echo "EDITOR_OR_GAME on compile.sh:       '${EDITOR_OR_GAME}'"
echo "RELEASE_OR_DEBUG on compile.sh: '${RELEASE_OR_DEBUG}'"
echo "-----"
sleep 2

export BANG_BUILD_EDITOR_OR_GAME="${EDITOR_OR_GAME}"
export BANG_BUILD_RELEASE_OR_DEBUG="${RELEASE_OR_DEBUG}"

# QMAKE & MAKE !!!
qmake "EDITOR_OR_GAME=${EDITOR_OR_GAME}" "RELEASE_OR_DEBUG=${RELEASE_OR_DEBUG}" 
make -j${NUM_THREADS}
