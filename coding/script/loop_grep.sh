#!/bin/bash

for l in `ls *.jar`
do
	unzip -l $l | grep -i notice
	if [ $? -eq 0 ] ; then
		echo $l
	fi
done
