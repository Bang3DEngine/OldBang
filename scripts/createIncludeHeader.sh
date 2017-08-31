#!/bin/bash

cd "$(dirname "$0")"
cd .. # Go to root directory

source "scripts/messages.sh"

HEADER_FILENAME=$1
INCLUDE_PROJ=$2

if [ -z $HEADER_FILENAME ] || [ -z $INCLUDE_PROJ ]
then
	Error "USAGE Please pass this args:"
	Error "    FIRST ARG:  the HEADER FILENAME you want to create the include to"
	Error "    SECOND ARG: the NAME OF THE PROJECT you want the created include to be in."
	exit 1
fi


INCLUDE_PROJ=$(find ./include/Bang -type d -name "$INCLUDE_PROJ" | head -n1)
if [ -z $INCLUDE_PROJ ] ; then Error "Include project '$2' not found..." ; exit 1 ; fi

cd $INCLUDE_PROJ

HEADER_FILE=$(find ../../../src -type f -name "$HEADER_FILENAME" | head -n1)
if [ -z $HEADER_FILE ] ; then Error "File '$HEADER_FILENAME' not found... (remember to add \".h\")" ; exit 1 ; fi

INCLUDE_FILE="Bang/$HEADER_FILENAME"

echo "#include \"Bang.h\"" > $INCLUDE_FILE
echo "#include \"$HEADER_FILE\"" >> $INCLUDE_FILE

Success "Created include:"
Success "   - From header '$HEADER_FILE'"
Success "   - Into project '$INCLUDE_PROJ'"
Success "   - In include file '$PWD/$INCLUDE_FILE'..."
echo "   - With contents:"
cat $INCLUDE_FILE 

exit 0
