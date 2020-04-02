#!/bin/bash
declare VAR="/usr/local/bin/UnitSW"

ID=$(cat "${VAR}/uid")
#ID="c0112233"
HEADER=$(curl -s -I http://89.103.230.142:9999/$ID)
FILELAST=$(cat "${VAR}/config_header.ini" | grep Last-Modified)
HEADERLAST=$(echo "$HEADER" | grep Last-Modified:)
CODE=$(echo "$HEADER" | grep HTTP  | cut -d' ' -f 2)

if [[ $CODE == "200" ]] && [[ $HEADERLAST != $FILELAST ]];
then
        curl -s -H 'Accept: application/json' -I http://89.103.230.142:9999/$ID --output "${VAR}/config_header.ini"
        curl -H 'Accept: application/json' http://89.103.230.142:9999/$ID --output "${VAR}/config.ini"
        sudo systemctl restart UnitSW.service
fi
