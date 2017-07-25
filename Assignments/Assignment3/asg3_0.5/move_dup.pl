#!/usr/bin/perl -w

# COMMENTS: move duplicate files. First input argument as the text file output
#	from fdupes. Second input argument a folder to move the dup files to
#NOTE: check $_ is a file (not directory), check $ARGV[1] has tailing / (for folder)

use strict;
use Cwd;
use File::Copy "move";
use File::Basename;


# check argument
if (@ARGV < 2) {
	die "Not enough input argument";
}

open (FILE, "<", $ARGV[0]) or die ("Cannot open file");		# open file
my $in_dup_set = 0;		# flag to keep track if in same duplicate set
my $pwd = cwd();		# get current working directory
while (defined ($_ = <FILE>)) {
	if ($in_dup_set == 0){
		chomp $_;	# remove \n
		my $basename = basename($_);	# get base name
		my $src = $pwd."/".$_;
		my $dest = $pwd."/".$ARGV[1]."/".$basename;
		move ($src, $dest) or die "Move file failed!"; 	# move to dup folder
		$in_dup_set = 1;
	}
	else {
		if ($_ eq "\n"){	# end of dup set
			$in_dup_set = 0;
		}
		else {
			chomp $_;	# remove \n
			unlink $pwd."/".$_;	# remove the file
		}
	}
}

close (FILE);
