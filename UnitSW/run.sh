#!/bin/bash
ps -ef | grep 'Reader' | grep -v grep | awk '{print $2}' | xargs -r kill -9
ps -ef | grep 'Aggregation' | grep -v grep | awk '{print $2}' | xargs -r kill -9

sleep 200
while true
do
  reader_pid=$(pidof Reader)
  aggregation_pid=$(pidof Aggregation)

  if [[ "$aggregation_pid" =~ ^[0-9]+$ ]] && [[ "$reader_pid" =~ ^[0-9]+[[:space:]][0-9]+$ ]]; then
    sleep 300
  else
      echo "nejede, zapinam"
      sudo bash /usr/local/bin/UnitSW/start.sh &
      sleep 10
  fi

done

