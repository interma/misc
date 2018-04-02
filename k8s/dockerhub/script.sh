#!/bin/bash

echo "There are $# parameters provided!"

for i in $@
do
	echo $i
done
