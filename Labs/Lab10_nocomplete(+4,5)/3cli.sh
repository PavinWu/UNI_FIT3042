#!/bin/bash
# f does not work
echo "Welcome to retro computing shell"
echo "Please enter a command"

function msg {
	echo -e "h: print this help message"
	echo -e "e filename: edit filename with vi"
	echo -e "t filename: touch this filename"
	echo -e "q: quit to bash shell"
	echo -e "l: list c source files"
	echo -e "f: find files whose names contain pattern"
}

option_cut=h
while [ $option_cut != q ]; do
	echo "Command: "
	read option
	option_cut=$( echo $option | head -c 1 )	# take only first char
	if [ $option_cut == h ]; then
		echo "$( msg )"		# Need to quote this or newline in msg won't get interpretted!
	elif [ $option_cut == e ]; then
		vi "$( echo $option | tail -c +3 )"
	elif [ $option_cut == t ]; then
		touch "$( echo $option | tail -c +3 )"
	elif [ $option_cut == l ]; then
		find ~ -name "*.[ch]"
	elif [ $option_cut == f ]; then
		name_option="$( echo $option | tail -c +3 )"
		name_option="$( echo $name_option | head -c -1 )"
		find . -name $name_option
		#echo "$( find . -name $name )"
	else
		echo "	quitting CLI"
	fi
done

