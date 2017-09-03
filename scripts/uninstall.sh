#!/bin/bash

cd "$(dirname "$0")"
cd .. # Go to root directory

source "scripts/messages.sh"

BeginHeader "UNINSTALLING"

if [ ! -f /usr/lib/libBang.so ]; then
   	Warning "Bang was not installed, so not uninstalling it..."
else
	echo "Removing include files from /usr/include..."
	rm  -rf /usr/include/Bang

	if [ $? != 0 ] ; then 
	Error "Could not uninstall. Need sudo! Please rerun with sudo!"
	else
	Success "Successfully uninstalled!"
	fi

	echo "Removing libBang.so from /usr/lib..."
	rm /usr/lib/libBang.so
fi

EndHeader

exit 0
