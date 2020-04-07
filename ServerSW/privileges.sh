#!/bin/sh

databases=$(influx -username admin -password bp_influx -execute "show databases;")
databases=$(echo "$databases" | grep -Eio '^([0-9]|[a-f]){8}$')
query=""
for tmp in $databases
do
        echo "Giving privileges for: $tmp, to: Chronograf = READ"
#	influx -username admin -password bp_influx -execute "GRANT READ ON "$tmp" TO Chronograf;"
	query+="GRANT READ ON $tmp TO Chronograf;"
	let "number_database+=1"
done

influx -username admin -password bp_influx -execute "$query"

echo "Number of databases: $number_database"

