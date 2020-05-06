#!/bin/sh
#while read in; do sh run-command.sh "$in" 8000; done < test.sql
#kill -USR1 `cat /tmp/sudba/sudba.pid`
#echo "delete from students;" | curl telnet://localhost:8000
echo "$1" | curl telnet://localhost:8001
