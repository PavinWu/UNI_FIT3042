#!/bin/bash
# NOTE: directory does not work (?)
pass=1
if [ $# -eq 0 ]; then
	dir='.'
elif [ $# -eq 1 ]; then
	dir=$1
else
	echo "USage"
	pass=0
fi

#grep_out=$( grep -rnHE "TODO:|FIXME:" ${dir}/*.[ch] ) | wc -l
#wc -l < $grep_out 
grep_out="$( grep -rnoHE "TODO:|FIXME:" "${dir}"/*.[ch] )"
grep_out="${grep_out//:TODO:/}"
echo "${grep_out//:FIXME:/}"
# echo ${grep_out} | wc -l    (need quotes! ?? (to handle whitespace))

echo "Your todo list has $( echo "${grep_out}" | wc -l ) items on it."
