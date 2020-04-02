#!/bin/bash

# A POSIX variable
OPTIND=1         # Reset in case getopts has been used previously in the shell.

# Initialize our own variables:
NUMBER_OF_UNITS=-1
TEST_NUMBER=-1
DELAY=-1

while getopts "u:d:t:h" opt; do
    case "$opt" in
    u)  NUMBER_OF_UNITS=$OPTARG
        ;;
    d)  DELAY=$OPTARG
        ;;
    t)  TEST_NUMBER=$OPTARG
        ;;
    h)  echo "-u <number of units> -t <test number> -m <number of messages> -l <delay between messages>";exit 1
		;;
    esac
done

re='^[0-9]+$'
if ! ( [[ $NUMBER_OF_UNITS =~ $re ]] && [[ $DELAY =~ $re ]] && [[ $TEST_NUMBER =~ $re ]]); then
   echo "error: Not a number" >&2; exit 1
fi

NUMBER_OF_MESSAGES=$((600000/$DELAY))
MESSAGES_PER_MINUTE=$(($NUMBER_OF_MESSAGES/10))

#echo $NUMBER_OF_MESSAGES
#echo $MESSAGES_PER_MINUTE
#echo $TEST_NUMBER
printf "%8s %15s %10s %10s %10s %10s %10s %10s %10s %10s %10s %10s\n" "Unit" "received/send" "0-1min" "1-2min" "2-3min" "3-4min" "4-5min" "5-6min" "6-7min" "7-8min" "8-9min" "9-10min" >> output.txt


for i in $(seq 0 $(($NUMBER_OF_UNITS-1)));
do
	TIME_START=0
	TIME_END=$(($TIME_START+59999999999*(1000/$DELAY)))
	DATABASE_NAME=$(printf f%03x%04x "$TEST_NUMBER" "$i")
	declare -A RESULT_ARRAY
	ANSWER=$(curl -H "Accept: application/csv" -XPOST "http://localhost:8086/query?db=$DATABASE_NAME&u=admin&p=bp_influx" --data-urlencode "q=SELECT COUNT(*) FROM sensor_0_rms_delta")
	RECEIVED_MESSAGES=$(echo $ANSWER | grep -Eo '[0-9]+$')
	RECEIVED_MESSAGES=$((RECEIVED_MESSAGES/12))

	for j in $(seq 1 10);
	do
		echo ""
		ANSWER=$(curl -H "Accept: application/csv" -XPOST "http://localhost:8086/query?db=$DATABASE_NAME&u=admin&p=bp_influx" --data-urlencode "q=SELECT COUNT(*) FROM sensor_0_rms_delta WHERE TIME >= $TIME_START AND TIME < $TIME_END")
		RESULT_ARRAY[$(($j-1))]=$(($(echo $ANSWER | grep -Eo '[0-9]+$')/12))

		TIME_START=$(($TIME_START+60000000000*(1000/$DELAY)))
		TIME_END=$(($TIME_START+59999999999*(1000/$DELAY)))
	done

	
	printf "%8s %15s %10s %10s %10s %10s %10s %10s %10s %10s %10s %10s\n"\
	"$DATABASE_NAME" \
	"$RECEIVED_MESSAGES/$NUMBER_OF_MESSAGES" \
	"${RESULT_ARRAY[0]}/$MESSAGES_PER_MINUTE" \
	"${RESULT_ARRAY[1]}/$MESSAGES_PER_MINUTE" \
	"${RESULT_ARRAY[2]}/$MESSAGES_PER_MINUTE" \
	"${RESULT_ARRAY[3]}/$MESSAGES_PER_MINUTE" \
	"${RESULT_ARRAY[4]}/$MESSAGES_PER_MINUTE" \
	"${RESULT_ARRAY[5]}/$MESSAGES_PER_MINUTE" \
	"${RESULT_ARRAY[6]}/$MESSAGES_PER_MINUTE" \
	"${RESULT_ARRAY[7]}/$MESSAGES_PER_MINUTE" \
	"${RESULT_ARRAY[8]}/$MESSAGES_PER_MINUTE" \
	"${RESULT_ARRAY[9]}/$MESSAGES_PER_MINUTE" >> output.txt
done