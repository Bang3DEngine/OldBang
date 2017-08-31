#!/bin/bash

cd "$(dirname "$0")"
cd .. # Go to root directory

source "scripts/messages.sh"

BeginHeader "UNINSTALLING"

if [ ! -f /usr/lib/libBang.so ]; then
   	Warning "Bang was not installed, so not uninstalling it..."
else
	echo "Removing libBang.so from /usr/lib..."
	rm /usr/lib/libBang.so
	if [ $? != 0 ] ; then 
	Error "Could not uninstall. Need sudo! Please rerun with sudo!"
	else
	Error "Successfully uninstalled!"
	fi
fi

EndHeader

exit 0
