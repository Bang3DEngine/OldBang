#!/bin/bash

cd "$(dirname "$0")"
cd .. # Go to root directory

source "scripts/captureArgs.sh"

BeginHeader "CLEANING"

echo "Removing build${RELEASE_OR_DEBUG}..."
rm -rf build${RELEASE_OR_DEBUG}
if [ $? != 0 ] ; then Error "ERROR" ; exit 1 ; fi

echo "Removing bin/${RELEASE_OR_DEBUG}..."
rm -rf bin/${RELEASE_OR_DEBUG}
if [ $? != 0 ] ; then Error "ERROR" ; exit 1 ; fi

Success "Successfully cleaned!"

EndHeader
