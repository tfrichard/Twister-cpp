#! /bin/bash

for line in `cat $TWISTER_HOME/bin/nodes`;do
	echo $line
	ssh $line pkill twister_daemon
	sleep 1
done
