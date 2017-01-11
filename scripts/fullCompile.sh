#!/bin/bash
cd "$(dirname "$0")"

echo "Full compiling will take a little longer..."
echo "You have 5 seconds to stop it."
printf "5..."; sleep 1 ; printf "4..."; sleep 1 ; printf "3..."; sleep 1 ; printf "2..."; sleep 1 ; printf "1..."; sleep 1 ; echo "Go!"
(cd .. ; qmake "BUILD_MODE=$1" ; make clean; rm Qt_MOCs/* ; rm bin/objEditor/* ; rm bin/objGame/* )
./compile.sh $1
