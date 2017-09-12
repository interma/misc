#!/bin/bash
#set -x

value=''

function get_value_bytag() {
	f=$1
	tag=$2
	value=`cat $f | tr '\n' ' ' | awk -F '<property>' '{ for(i = 1; i <= NF; i++) { print $i; } }' | grep $tag | sed -n 's|.*<value>\(.*\)</value>.*|\1|p'`
	echo $value
	#echo "XXX"
}

#get_value_bytag test.xml hawq_master_address_host
#echo $value
val=$(get_value_bytag test.xml hawq_master_address_host)
echo $val
