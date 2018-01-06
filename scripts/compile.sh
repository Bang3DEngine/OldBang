#!/bin/bash

cd "$(dirname "$0")"
cd .. # Go to root directory

source "scripts/captureArgs.sh"
source "scripts/compileTargets.sh" "${BUILD_TYPE}" BangSharedLib

