#!/bin/sh

databases=$(influx -username admin -password bp_influx -execute "show databases;")
databases=$(echo "$databases" | grep -Eio '^dat_([0-9]|[a-f]){8}$')

for tmp in $databases
do
        echo "Giving privileges for: $tmp, to: Chronograf = READ"
	influx -username admin -password bp_influx -execute "GRANT WRITE ON "$tmp" TO Chronograf;"
	let "number_database+=1"
done

echo "Number of databases: $number_database"

