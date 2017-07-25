#!/bin/bash
if [ $# -eq 0 ]; then		# no argument
	echo "Number of file is $( ls | wc -w )"
elif [ $# -eq 1 ]; then
	if [ -d $1 ]; then
		echo "Number of file is $( ls $1 | wc -w )"
	else 
		echo "Nooooooo!"	
	fi
else
	echo "usage"
fi
