#!/bin/bash

cd "$(dirname "$0")"
cd .. # Go to root directory

HEADER_FILENAME=$1
INCLUDE_PROJ=$2

if [ -z $HEADER_FILENAME ] || [ -z $INCLUDE_PROJ ]
then
	echo "USAGE Please pass this args:"
	echo "    FIRST ARG:  the HEADER FILENAME you want to create the include to"
	echo "    SECOND ARG: the NAME OF THE PROJECT you want the created include to be in."
	exit 1
fi


INCLUDE_PROJ=$(find ./include/Bang -type d -name "$INCLUDE_PROJ" | head -n1)
if [ -z $INCLUDE_PROJ ] ; then echo "ERROR: Include project '$2' not found..." ; exit 1 ; fi

cd $INCLUDE_PROJ

HEADER_FILE=$(find ../../../src -type f -name "$HEADER_FILENAME" | head -n1)
if [ -z $HEADER_FILE ] ; then echo "ERROR: File '$HEADER_FILENAME' not found... (remember to add \".h\")" ; exit 1 ; fi

INCLUDE_FILE="Bang/$HEADER_FILENAME"

echo "#include \"Bang.h\"" > $INCLUDE_FILE
echo "#include \"$HEADER_FILE\"" >> $INCLUDE_FILE

echo "Created include:"
echo "   - From header '$HEADER_FILE'"
echo "   - Into project '$INCLUDE_PROJ'"
echo "   - In include file '$PWD/$INCLUDE_FILE'..."
echo "   - With contents:"
cat $INCLUDE_FILE 

exit 0
