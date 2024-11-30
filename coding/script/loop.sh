#!/bin/bash
set -x
while [ "xx" == "xx" ]
do
	#ls .
	ls xx
	if [ "$?" != "0" ] ; then
		echo "exit"
		exit -1
	fi
done
