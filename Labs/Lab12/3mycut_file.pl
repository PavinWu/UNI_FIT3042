#!/usr/bin/perl -w
use strict;

($ARGV[0] =~ /^[1-9]$/) or
	die("Take 1-9 fields");

my @ranges = @ARGV;
shift @ranges;
pop @ranges;

open(FILE, "<", $ARGV[$#ARGV]);

while (<FILE>) {
	my @data = split /,/;
	##WHY NOT WORK WITH CHOMPPPPP!!!!!?????
	if (@data < $ARGV[0]){
		print "Not enough input fields\n";
		next;
	}
	my $count = 0;
	foreach (@ranges) {
		my $ind; my $end;
		if(/^([1-9])\-?[1-9]?$/){
			$ind = $1;
			if(/^([1-9])\-$/){
				$end = @data;
			}
			elsif(/^([1-9])\-([1-9])$/){
				$end = $2;
			}
			else{
				$end = $1;
			}
		}
		elsif(/^\-([1-9])$/){
			$ind = 1;
			$end = $1;
		}
		else {
			die("Invalid range");
		}
		
		for($ind; $ind<$end; $ind++){
			print $data[$ind-1], ",";
		}
		print $data[$end-1];
		if ($count < $#ranges){
			print ",";
		}
		$count++;
	}
	
}
print "\n";
close (FILE);