#!/bin/bash

exceptionsList=($*)
echo "qmakeAll exceptions: $exceptionsList"

qmakeFiles="$(find . | grep -E "\.pro$" | grep "Bang" | xargs)"
filesList=($qmakeFiles)
for f in "${filesList[@]}"
do
	process=""
	for exception in "${exceptionsList}"
	do
		exception="./${exception}"
		if [[ "$exception" == "$f" ]] ; then process="nope" ; fi
	done

	if [ -z $process ]
	then
		qmake "$f"
	fi
done
