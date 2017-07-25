#!/usr/bin/perl

use warnings;
use strict;

use File::Find;

my %hash = ();

sub countExt {
	#$_ is set to the current file name
	#http://world.std.com/~swmcd/steven/perl/pm/find.html
	
	my $name = $_;
	print $name, "\n";
	#if (/*.([a-zA-Z0-9._]+)/){	#be careful, not bash globbing!
	if (/.*\.([a-zA-Z0-9._]+)/){
		if (exists $hash{$1}){
			$hash{$1}++;
		}
		else {
			$hash{$1} = 1;
		}
	}
}

if (-e $ARGV[0] and -d $ARGV[0]){
	find(\&countExt, $ARGV[0]);		# do countExt as find traverses directory tree
	print "\n";
	my @extensions = keys %hash;
	@extensions = sort @extensions;
	foreach (@extensions){
		print $_, ": ", $hash{$_}, "\n";
	}
}
else {
	die "directory not exists";
}