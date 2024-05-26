#!/usr/bin/env bash
# #============================#
# | main implementation script |
# #============================#
BIN_DIR=bin
WIN_MODE="n"
if [ "${OS}" = "Windows_NT" ]; then
	RAW_BIN_NAME=l61.exe
    GCC_ARC=x86_64-w64-mingw32-
else
    if [ "${WIN_MODE}" = "y" ]; then
	RAW_BIN_NAME=l61.exe
    GCC_ARC=x86_64-w64-mingw32-
    else
    RAW_BIN_NAME=l61
    GCC_ARC=
    fi
fi

SCRTPTS=scripts