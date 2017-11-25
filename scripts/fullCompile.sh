#!/bin/bash

cd "$(dirname "$0")"
cd .. # Go to root directory

source "scripts/captureArgs.sh"
source "scripts/clean.sh" "${BUILD_TYPE}"
source "scripts/compileExecutables.sh" "${BUILD_TYPE}"
