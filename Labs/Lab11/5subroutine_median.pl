#!/usr/bin/perl -w
use strict;

sub median { my @num_list = @_;
	@num_list = sort {$a <=> $b} @num_list; 	# sort with ASCII otherwise
	my $len = @num_list; #length @num_list;
	#my $len = $#num_list + 1;	# gives last element's index
	if($len % 2){
		return $num_list[$len/2];
	} else {
		return ($num_list[$len/2]+$num_list[$len/2-1])/2;
	}
	
}

$#ARGV != -1 or die("empty argument list");
print median @ARGV;
