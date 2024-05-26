#!/usr/bin/env bash

if [ -e "/usr/bin/doas" ] ; then
    DOAS=doas
else
    DOAS=sudo
fi

${DOAS} cp -rf ./bin/debugger.lex61 /opt/l61/lib/debugger.lex61
echo "installed ldb++"