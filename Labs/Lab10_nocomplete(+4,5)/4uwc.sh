#/bin/bash

# go through file, for each word check if word already exists in temp
# put there if not and put num to 1, otherwise increment	(separate number and word by space)
# then use cut to get only second field

# can actually just write word to the temp file and use grep to find (we will put one word per line => no need to worry about multiple match in line)
# don't have to count either

# not error checked!

file="$1"	# assume words are space separated.

words_str="$( cat $file )"	# ----!- result will be space separated ( already form of array ) ------
# don't have to quote if already quote! (what if has spaced file name???)
words=( $words_str )		# !!!!! make array! (no quote!)

wc_file="/tmp/file$$"
echo "" > "$wc_file"		# !!!!! Create first!
for word in "${words[@]}"; do		# put words into file	!!!!! use ${words[@]} !
	echo -e "${word}\\n" >> "$wc_file"
done


#while file_word not empty, get word from file_word and grep that word in wc_file (and count with wc -l), then delete word from file_words
# ... can't "Dynamically delete word from the thing that is used as loop condition
# delete word from wc_file instead, and check also that word is not in wc_file before grep
for word in "${words[@]}"; do
	num_word="$( grep $word $wc_file | wc -l )"
	if [ $num_word -ne 0 ]; then
		echo ${word}: ${num_word}
		grep -v "$word" $wc_file > "${wc_file}_2"	#!!!!! $wc_file_2 means variable of that name (not what we want!)
		cp "${wc_file}_2" $wc_file
	fi
done

rm $wc_file $wc_file_2

#https://unix.stackexchange.com/questions/66889/minimal-command-to-make-a-copy-of-a-file
# cp $wc_file $wc_file_2	# duplicate

#file_words="$( cat $wc_file )"
#for word in "$file_words"; do
#	word=${word%\\n}	# remove \\n
#	echo "$word : $( grep $word $wc_file | wc -l )\\n"
#	grep -v $word 
#done


