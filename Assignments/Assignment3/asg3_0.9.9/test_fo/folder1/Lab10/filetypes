#!/bin/bash
pass=1
if [ $# -eq 0 ]; then
	dir='.'
elif [ $# -eq 1 ]; then
	dir=$1
else
	echo "USage"
	pass=0
fi

if [ $pass -eq 1 ]; then
	test="p b c d l f"
	for t in $test; do
		echo "$t: $( find $dir -maxdepth 1 -type $t | wc -w )"
	done
fi
