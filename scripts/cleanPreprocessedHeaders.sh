#!/bin/bash

cd "$(dirname "$0")"
cd .. # Go to root directory

HEADERS_DIR="include/Bang"
echo "Cleaning preprocessed headers..."
rm -f $(find . | grep -E "gen\.h" | xargs)
echo "Preprocessed headers cleaned!"
