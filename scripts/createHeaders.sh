#!/bin/bash

cd "$(dirname "$0")"
cd .. # Go to root directory

mkdir -p include/Bang

for f in $(find . | grep "\.h" | xargs); 
do 
	filename=$(echo $(basename "$f"));
	echo "#include \"../../$f\"" > "./include/Bang/${filename}"
done


