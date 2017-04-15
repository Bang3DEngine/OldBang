#!/bin/bash

cd "$(dirname "$0")"
cd .. # Go to root directory

HEADERS_DIR="include/Bang"
echo "Creating headers in '${HEADERS_DIR}'"
mkdir -p "${HEADERS_DIR}"

FILES="$(find . | grep -E ".*/src/.*/[A-Z][A-Za-z0-9_-]*\.h$" | xargs)"
#FILES="$FILES $(find . | grep -E "ui_.*\.h$"  | xargs)"
for f in $FILES 
do 
	filename=$(echo $(basename "$f"));
	filepath="${HEADERS_DIR}/${filename}"
	: >  "${filepath}"
	echo "#include \"Bang.h\""   >> "${filepath}"
	echo "#include \"../../$f\"" >> "${filepath}"
done


