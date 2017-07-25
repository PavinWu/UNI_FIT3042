#!/usr/bin/perl -w
use strict;

sub factorial {
	# $_ = $_[0];
	# print $_, " ";
	# won't get multiplied if just use $_ (?)
	
	/^[0-9]+$/ or die("Not integer input");
	
	if ($_[0] == 1) {
		return 1;
	}
	else {
		return $_[0]*factorial($_[0]-1);
	}
	
}

print factorial @ARGV;
print "\n";

