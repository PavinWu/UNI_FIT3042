#!/usr/bin/perl -w

# restore_dup.pl for undear: undedupping unarhiever
# ------------------------------------------
# This restores duplicate files from the one copy of each duplicate set.
# This deals with duplicates with different names in similar manner to delete_dup.pl
#
# Written by Pavin Wu (26916592)
# Written on 20 May 17

# NOTE: this script takes two arguments: 
#	$ARGV[0]: flag, $ARGV[1]: dup_files_list.txt

use strict;
use Cwd;
use File::Copy;
use File::Basename;

# check number of argument
if (@ARGV < 2){
	die "Not enough input argument for perl script";
}

my $pwd = cwd();	# get current directory
my $flag = $ARGV[0];	
open (FILE, "<", $ARGV[1]) or die ("Cannot open file");		# open dup_files_list.txt

my %namehash = ();		# initialize hash to contain unique names in each dup set and their paths
while (defined ($_ = <FILE>)) {
	if ($_ ne "\n"){	# if not new line (i.e. beginning different set of dup)
		chomp $_;		# remove \n
		my $basename = basename($_);
		
		if (defined $namehash{$basename}){		# if basename in hash with path
			my $origfile = $namehash{$basename};	# path to file of orig basename from hash
			if ($flag eq "-l"){			# make link
				system(("ln", "-s", $pwd."/".$origfile, dirname($_)))
				# need absolute path or link will make reference relative to dirname($_)
				# i.e. symlink at dirname($_) will link to dirname($_).$origfile (not exists)
			}
			elsif ($flag eq "-c") {		# copy
				copy($origfile, dirname($_)) or 
					warn "Copy file failed for $origfile";
				# don't have linking constaint, can use relative path for copy process
			}
		}
		else {	# basename not in hash: insert basename and associating directory to hash
			$namehash{$basename} = $_;		# basename as key, full path (includes name) as value
		}			
	}
	else {	# end of dup set
		%namehash = ();		# empty hash
	}
}

close (FILE);
