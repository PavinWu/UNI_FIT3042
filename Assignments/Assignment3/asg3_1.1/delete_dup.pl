#!/usr/bin/perl -w

# delete_dup.pl for dear: dedupping arhiever
# ------------------------------------------
# This deletes duplicate files while keeping one copy from each duplicate set.
# Since fdupes treat duplicate files with different names to be duplicates,
#	this script keeps track of those files and treat them as if they are different.
#	This is required to reconstruct original dir (almost (i.e. still different file's metadata)) exactly.
#	Other methods may be used but this method does not require extra file manipulation.
#
# Written by Pavin Wu (26916592)
# Written on 20 May 17

# NOTE: This script must be in same directory as dear (so it may be copied by dear
#	into directory of [directory to be archived]). Do this for simple path processing.
# NOTE: This script takes a single argument:
#	$ARGV[0]: dup_files_list.txt (output from fdupes).

use strict;
use File::Basename;

# check number of argument
if (@ARGV < 1) {
	die "Not enough input argument for perl script";
}

# open file
open (FILE, "<", $ARGV[0]) or die ("Cannot open file");		

my @basenames = ();		# initialize basename for duplicate with different names
while (defined ($_ = <FILE>)) {
	if ($_ ne "\n"){	# if not new line (i.e. beginning different set of dup)
		chomp $_;		# remove trailing \n
		my $basename = basename($_);	# new basename
		my $havename = 0;	
		foreach my $prev_basename (@basenames) {
			if ($basename eq $prev_basename) {
				$havename = 1;
				last;	# break
			}
		}
		if ($havename == 1){
			unlink $_;					# remove the file
		}
		else {
			push (@basenames, $basename);	# push new basename to array
		}			
	}
	else {	# end of dup set
		@basenames = ();		# empty array
	}
}

close (FILE);
