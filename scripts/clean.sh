#!/bin/bash

cd "$(dirname "$0")"
cd .. # Go to root directory

source "scripts/captureArgs.sh"

BeginHeader "CLEANING"

echo "Removing build${BUILD_TYPE}..."
rm -rf build${BUILD_TYPE}
if [ $? != 0 ] ; then Error "ERROR" ; exit 1 ; fi

echo "Removing bin/${BUILD_TYPE}..."
rm -rf bin/${BUILD_TYPE}
if [ $? != 0 ] ; then Error "ERROR" ; exit 1 ; fi

Success "Successfully cleaned!"

EndHeader
