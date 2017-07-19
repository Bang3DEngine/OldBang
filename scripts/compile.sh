#!/bin/bash

cd "$(dirname "$0")"
cd ../proj # Go to root directory

export QT_SELECT=5

NUM_THREADS=6
RELEASE_OR_DEBUG=$1

echo "-----"

ERROR=0

if [ "${RELEASE_OR_DEBUG}" != "DEBUG" ] && [ "${RELEASE_OR_DEBUG}" != "RELEASE" ]
then
	echo "Option '${RELEASE_OR_DEBUG}' for the first parameter not recognised. Valid options are:"
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
echo "RELEASE_OR_DEBUG on compile.sh: '${RELEASE_OR_DEBUG}'"
echo "-----"
sleep 2

export BANG_BUILD_RELEASE_OR_DEBUG="${RELEASE_OR_DEBUG}"

# QMAKE & MAKE !!!
qmake
make -j${NUM_THREADS}
