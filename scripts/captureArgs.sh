#!/bin/bash

RELEASE_OR_DEBUG=$1
if [ "${RELEASE_OR_DEBUG}" != "Debug" ] && [ "${RELEASE_OR_DEBUG}" != "Release" ]
then
        echo "Option '${RELEASE_OR_DEBUG}' for the first parameter not recognised. Valid options are:"
        echo "   Debug"
        echo "   Release"
        exit 1
fi

