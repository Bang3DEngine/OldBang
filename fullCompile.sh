#!/bin/bash

sleep 5

./updateUI.sh

# Make and run Bang
rm obj*/*

./compile.sh $1
