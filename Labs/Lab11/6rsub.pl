#!/usr/bin/perl -w
#use strict;
use Scalar::Util qw(looks_like_number);

sub fact {
	#$num = @_;	#array evaluated in scalar context
	# promoted to array size[0] == 1){
	# HOW TO USE @num IN HERE????
	
	if ($_[0] == 1){	#an "element of" @_ (NOT same as $_)
		return 1;
	}
	else {
		return $_[0]*fact($_[0]-1);
	}
}

if (looks_like_number($ARGV[0])){
	print fact($ARGV[0]),"\n";
}
else {
	print "input not a number";	
}
