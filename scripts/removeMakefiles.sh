#!/bin/bash

rm $(find . | grep -E "Makefile$" | xargs)
