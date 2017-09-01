#!/bin/bash

cd "$(dirname "$0")"
cd .. # Go to root directory

source "scripts/captureArgs.sh"

./scripts/compileLibs.sh ${RELEASE_OR_DEBUG} $*
if [ $? != 0 ] ; then exit 1 ; fi

./scripts/compileExecutables.sh ${RELEASE_OR_DEBUG} $*
if [ $? != 0 ] ; then exit 1 ; fi

echo;echo
BeginHeader "INSTALLING"

echo "Copying Bang engine into /opt/Bang..."
mkdir -p /opt/Bang
cp -r res scripts /opt/Bang

echo "Copying include's into /usr/include..."
cp -r include /usr/include

echo "Copying libBang.so into /usr/lib..."
cp bin/${RELEASE_OR_DEBUG}/lib/libBang.so /usr/lib
if [ $? != 0 ] ; then 
Error "Could not install. Need sudo! Please rerun with sudo!"
else
Success "Successfully installed!"
fi

EndHeader

exit 0
