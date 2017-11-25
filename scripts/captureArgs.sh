#!/bin/bash

source "scripts/messages.sh"

BUILD_TYPE=$1
if [ "${BUILD_TYPE}" != "Debug" ] && [ "${BUILD_TYPE}" != "Release" ] && [ "${BUILD_TYPE}" != "RelWithDebInfo" ]
then
        Error "Option '${BUILD_TYPE}' for the first parameter not recognised. Valid options are:"
        Error "   Debug"
        Error "   Release"
        Error "   RelWithDebInfo"
        exit 1
fi
shift

BANG_COMPILER="${BANG_COMPILER}"
