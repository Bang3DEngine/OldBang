#!/bin/bash

cd res
for f in *.ui
do
	fname=$(echo $f | cut -d'.' -f1)
	uic $fname.ui > ../include/View/$fname.h
	echo "Created $fname.h" 
done
