#!/bin/bash

cd "$(dirname "$0")"
cd .. # Go to root directory

echo "Starting preprocessing of header files..."

bangPreprocessor="$(find bin | grep -E "\bBangPreprocessor$" | xargs | cut -d' ' -f1)"
echo "$bangPreprocessor"
if [ ! -f $bangPreprocessor ] || [ -z $bangPreprocessor ]
then
	echo "BangPreprocessor binary was not found. Ignoring preprocessing..."
	exit 0
fi
bangPreprocessor="./$bangPreprocessor"

FILES="$(find . | grep -E ".*/src/.*/[A-Z][A-Za-z0-9_-]*\.h$" | xargs)"
for f in $FILES
do
	$bangPreprocessor "$f"
done
