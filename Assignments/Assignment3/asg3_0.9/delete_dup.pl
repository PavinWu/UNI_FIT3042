#!/usr/bin/perl -w

# This script must be in same directory to main folder to be compressed

# COMMENTS: Use with dear. move duplicate files. First input argument as the text file output
#	from fdupes. Second input argument a folder to move the dup files to
#NOTE: check $_ is a file (not directory), check $ARGV[1] has tailing / (for folder)

use strict;
use Cwd;
use File::Basename;


# check argument
if (@ARGV < 1) {
	die "Not enough input argument";
}

open (FILE, "<", $ARGV[0]) or die ("Cannot open file");		# open file

my @basenames = ();		# initialize
while (defined ($_ = <FILE>)) {
	if ($_ ne "\n"){
		chomp $_;	# remove \n
		my $basename = basename($_);
		my $havename = 0;	# fdupes also gives dup files **with different names**. Need to check
		foreach my $prev_basename (@basenames) {
			if ($basename eq $prev_basename) {
				$havename = 1;
				last;	# break
			}
		}
		if ($havename == 1){
			unlink $_;			# remove the file. $_ has relative path
		}
		else {
			push (@basenames, $basename);
		}			
	}
	else {	# end of dup set
		@basenames = ();
	}
}

close (FILE);
