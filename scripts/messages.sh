#!/bin/bash

RED='\033[1;31m'
GREEN='\033[1;32m'
YELLOW='\033[1;33m'
CYAN='\033[1;36m'
NO_COLOR='\033[0m'

LAST_HEADER_LENGTH=0

function Success { 
	echo -e "${GREEN}[SUCCESS]: $1${NO_COLOR}"
}
function Warning { 
	echo -e "${YELLOW}[WARNING]: $1${NO_COLOR}" 
}
function Error {
	echo -e "${RED}[ERROR]: $1${NO_COLOR}" 
}
function BeginHeader { 
	HEADER="======= $1 ======="
	echo ; echo -e "${CYAN}${HEADER}${NO_COLOR}"
	LAST_HEADER_LENGTH=${#HEADER}
}
function EndHeader {
	HEADER_CLOSE=""
	for (( i=0; i<${LAST_HEADER_LENGTH}; i++ )) ; do
		HEADER_CLOSE="${HEADER_CLOSE}="
	done
	echo -e "${CYAN}${HEADER_CLOSE}${NO_COLOR}" ; echo
}
