#! /bin/bash
if [ $# -ne 1 ]
	then
	echo "Usage: driver_ip"
	exit $?
fi

i=0
for line in `cat $TWISTER_HOME/bin/nodes`;do
	echo $line
	ssh $line $TWISTER_HOME/bin/daemon.sh $i $line $1&
	i=`expr $i + 1`
	sleep 1
done
