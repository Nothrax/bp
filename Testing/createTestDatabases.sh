#!/bin/bash

# A POSIX variable
OPTIND=1         # Reset in case getopts has been used previously in the shell.

# Initialize our own variables:
NUMBER_OF_UNITS=-1
TEST_NUMBER=-1

while getopts "u:t:h" opt; do
    case "$opt" in
    u)  NUMBER_OF_UNITS=$OPTARG
        ;;
    t)  TEST_NUMBER=$OPTARG
        ;;
    h)  echo "-u <number of units> -t <test number>";exit 1
		;;
    esac
done

re='^[0-9]+$'
if ! ( [[ $NUMBER_OF_UNITS =~ $re ]] && [[ $TEST_NUMBER =~ $re ]] ); then
   echo "error: Not a number" >&2; exit 1
fi

TEST_NUMBER=$(printf "%03x" $TEST_NUMBER)

DELETE_QUERY=""
CREATE_QUERY=""
for i in $(seq 1 $NUMBER_OF_UNITS);
do
	UNIT_NUMBER=$(printf "%04x" $(($i-1)))
	DELETE_QUERY+="drop database f$TEST_NUMBER$UNIT_NUMBER;"
	CREATE_QUERY+="create database f$TEST_NUMBER$UNIT_NUMBER;"
	
done
influx -username admin -password bp_influx -execute "$DELETE_QUERY"
influx -username admin -password bp_influx -execute "$CREATE_QUERY"

echo "number of units  = ${NUMBER_OF_UNITS}"
echo "test number     = ${TEST_NUMBER}"
