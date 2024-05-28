#!/usr/bin/env bash

if [[ -e "/usr/bin/doas" ]]; then
    DOAS=doas
else
    DOAS=sudo
fi

if [[ $USER == "root" ]]; then
    DOAS=""
fi

${DOAS} cp -rf ./bin/debugger.lex61 /opt/l61/lib/debugger.lex61
if [[ "$?" -eq "1" ]]; then
    echo "doas error"
    exit 0
fi
echo "installed ldb++"