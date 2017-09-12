#!/bin/bash

cd "$(dirname "$0")"
cd .. # Go to root directory

source "scripts/messages.sh"

./scripts/compileLibs.sh Debug $*
./scripts/compileLibs.sh Release $*
if [ $? != 0 ] ; then exit 1 ; fi

./scripts/compileExecutables.sh Debug $*
./scripts/compileExecutables.sh Release $*
if [ $? != 0 ] ; then exit 1 ; fi

echo;echo
BeginHeader "INSTALLING"

echo "Copying Bang engine into /opt/Bang..."
mkdir -p /opt/Bang
cp -r res scripts /opt/Bang

echo "Copying include's into /usr/include..."
cp -r include/Bang /usr/include

echo "Copying libBang.so into /usr/lib..."
cp bin/Release/lib/libBang.so /usr/lib/libBang.so
cp bin/Debug/lib/libBang.so /usr/lib/libBangd.so
if [ $? != 0 ] ; then 
Error "Could not install. Need sudo! Please rerun with sudo!"
else
Success "Successfully installed!"
fi

EndHeader

exit 0
