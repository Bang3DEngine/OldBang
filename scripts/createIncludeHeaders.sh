#!/bin/bash

cd "$(dirname "$0")"
cd .. # Go to root directory

HEADERS_DIR="include/Bang"
echo "Creating headers in '${HEADERS_DIR}'"
mkdir -p "${HEADERS_DIR}"

FILES="$(find . | grep -E ".*/src/.*/[A-Z][A-Za-z0-9_-]*\.h$" | xargs)"
for f in $FILES 
do 
	includeHeaderFilename=$(echo $(basename "$f"));
	includeHeaderFilepath="${HEADERS_DIR}/${includeHeaderFilename}"
	
	if [ ! -f "$includeHeaderFilepath" ]
	then
		filename="$(basename "$f")"
		filename="${filename%.*}"
		filedir="$(dirname "$f")"

		echo "Creating header ${includeHeaderFilepath}..."
		: >  "${includeHeaderFilepath}"
		echo "#include \"Bang.h\"" >> "${includeHeaderFilepath}"
		echo "#include \"../../$f\"" >> "${includeHeaderFilepath}"
	fi
done


