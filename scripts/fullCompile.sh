#!/bin/bash
cd "$(dirname "$0")"

echo "Full compiling will take a little longer..."
echo "You have 5 seconds to stop it."
printf "5..."; sleep 1 ; 
printf "4..."; sleep 1 ; 
printf "3..."; sleep 1 ; 
printf "2..."; sleep 1 ; 
printf "1..."; sleep 1 ; 
echo "Go!"

echo "Clearing previous builds..." ############
if [ "$1" == "EDITOR" ]
then
	if [ "$2" == "DEBUG" ] ; then rm bin/objEditorDebug/* ; fi
	if [ "$2" == "RELEASE" ] ; then rm bin/objEditorRelease/* ; fi
fi

if [ "$1" == "GAME" ]
then
	if [ "$2" == "DEBUG" ] ; then rm bin/objGameDebug/* ; fi
	if [ "$2" == "RELEASE" ] ; then rm bin/objGameRelease/* ; fi
fi
rm Qt_MOCs/*
rm ui_*
rm -rf bin/*
###############################################

(cd .. ; qmake "EDITOR_OR_GAME=$1" "RELEASE_OR_DEBUG=$2" ; make clean) 2> /dev/null
echo "" ; echo "------------------------"; echo ""

./compile.sh "$1" "$2"
