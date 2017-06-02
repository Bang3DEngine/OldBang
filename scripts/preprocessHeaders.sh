#!/bin/bash

cd "$(dirname "$0")"
cd .. # Go to root directory

echo "Starting preprocessing of header files..."

if [ ! -f "bin/BangPreprocessor" ]
then
	echo "bin/BangPreprocessor does not exist. Ignoring preprocessing..."
	exit 0
fi

FILES="$(find . | grep -E ".*/src/.*/[A-Z][A-Za-z0-9_-]*\.h$" | xargs)"
for f in $FILES
do
	./bin/BangPreprocessor "$f"
done
