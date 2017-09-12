#!/bin/bash
old=$1
new=$2
for i in *;
do
	newname=`echo $i|sed "s/\.$old$/\.$new/"`
	echo $newname
	if [ $i != $newname ];then
		mv $i $newname
	fi
done
exit 0
