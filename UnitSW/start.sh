#!/bin/bash

ps -ef | grep 'Reader' | grep -v grep | awk '{print $2}' | xargs -r kill -9
ps -ef | grep 'Aggregation' | grep -v grep | awk '{print $2}' | xargs -r kill -9


sudo taskset -c 2,3 chrt -v -f 99 nice -n 20 /usr/local/bin/UnitSW/Reader/build/Reader -p /usr/local/bin/UnitSW/config.ini &

sleep 1
sudo /usr/local/bin/UnitSW/Aggregation/build/Aggregation -p /usr/local/bin/UnitSW/config.ini &

