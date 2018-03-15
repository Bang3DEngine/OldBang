#!/bin/bash

cd "$(dirname "$0")"
cd ../include

ALL_H_FILE="Bang/All.h"

echo "#ifndef ALL_H" > $ALL_H_FILE
echo "#define ALL_H" >> $ALL_H_FILE
echo "" >> $ALL_H_FILE

for f in Bang/*
do
    echo "#include \"$f\"" >> $ALL_H_FILE
done

echo "" >> $ALL_H_FILE
echo "#endif //ALL_H" >> $ALL_H_FILE 
