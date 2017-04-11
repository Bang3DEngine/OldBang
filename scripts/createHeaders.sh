#!/bin/bash

cd "$(dirname "$0")"
cd .. # Go to root directory

mkdir -p include/Bang

FILES="$(find . | grep -E ".*/src/.*/[A-Z][A-Za-z0-9_-]*\.h$" | xargs)"
FILES="$FILES $(find . | grep -E "ui_.*\.h$" | xargs)"
FILES="$FILES $(find . | grep -E "stb_.*\.h$" | xargs)"
for f in $FILES 
do 
	filename=$(echo $(basename "$f"));
	filepath="./include/Bang/${filename}"
	echo ""                      >  "${filepath}"
	echo "#include \"Bang.h\""   >> "${filepath}"
	echo "#include \"../../$f\"" >> "${filepath}"
done


