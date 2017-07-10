#!/bin/bash

rm -f $(find . | grep -E "Makefile$" | xargs)
