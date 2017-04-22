#!/bin/bash

cd "$(dirname "$0")"
cd ..

cov-build --dir cov-int ./scripts/fullCompile.sh EDITOR DEBUG_MODE
tar czvf Bang.tgz cov-int
