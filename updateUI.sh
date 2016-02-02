#!/bin/bash

cd View
for f in *.ui
do
	fname=$(echo $f | cut -d'.' -f1)
	uic $fname.ui > $fname.h
	echo "Created $fname.h" 
done
