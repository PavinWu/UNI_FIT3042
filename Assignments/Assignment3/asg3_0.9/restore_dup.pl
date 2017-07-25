#!/usr/bin/perl -w

## $ARGV[0] is flag, $ARGV[1] is dup_files_list.txt
# this pl in same dir as tar. Hence same as test_fo, dup_files_list.txt


use strict;
use Cwd;
use File::Copy;
use File::Basename;

if (@ARGV < 2){
	die "Not enough input argument";
}

my $flag = $ARGV[0];
open (FILE, "<", $ARGV[1]) or die ("Cannot open file");		# open dup_files_list.txt

my $crwd = cwd();	# get current working directory
my %namehash = ();		# initialize
while (defined ($_ = <FILE>)) {
	if ($_ ne "\n"){
		chomp $_;	# remove \n
		my $basename = basename($_);
		
		if (defined $namehash{$basename}){
			my $origfile = $namehash{$basename};
			if ($flag eq "-l"){
				!system(("ln", "-s", $crwd."/".$origfile, dirname($_))) or 	# ln return 0 if no error
					warn "Creating link failed for $origfile";
			}
			elsif ($flag eq "-c") {	
				copy($origfile, dirname($_)) or 
					warn "Copy file failed for $origfile";
			}
		}
		else {	# insert basename and associating directory to hash
			$namehash{$basename} = $_;		# basename as key, full path (includes name) as value
		}			
	}
	else {	# end of dup set
		%namehash = ();
	}
}

close (FILE);
