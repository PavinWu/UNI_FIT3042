#!/usr/bin/perl -w

#while (<STDIN>){
#	if (80 < length){
#		print scalar reverse;
#	}
#	else {
#		print;
#	}
#}

while ( defined($_ = <STDIN>)){	#put in brackets for a serial expressions
	if (20 < length($_)){
		print scalar reverse $_;	# $_ is a scalar string
	}
	else {
		print $_;
	}
}
