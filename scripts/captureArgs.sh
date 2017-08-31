#!/bin/bash

source "scripts/messages.sh"

RELEASE_OR_DEBUG=$1
if [ "${RELEASE_OR_DEBUG}" != "Debug" ] && [ "${RELEASE_OR_DEBUG}" != "Release" ]
then
        Error "Option '${RELEASE_OR_DEBUG}' for the first parameter not recognised. Valid options are:"
        Error "   Debug"
        Error "   Release"
        exit 1
fi
shift

BANG_COMPILER="${BANG_COMPILER}"
