#!/bin/bash

cd "$(dirname "$0")"
cd .. # Go to root directory

source "scripts/captureArgs.sh"

rm -rf build${RELEASE_OR_DEBUG}
rm -rf bin/${RELEASE_OR_DEBUG}
