#!/usr/bin/perl

use warnings;
use strict;
use diagnostics;

use File::Find;
use File::Basename;

my %hash = ();

sub countExt {
	#$_ is set to the current file name
	#http://world.std.com/~swmcd/steven/perl/pm/find.html
	
	print $_, "\n";
	#if (/*.([a-zA-Z0-9._]+)/){	#be careful, not bash globbing!
	if (/(.*)\.([a-zA-Z0-9._]+)/){
		if (exists $hash{$2}){
			push @{$hash{$2}}, basename($1);
		}
		else {
			$hash{$2} = [basename($1)];
		}
	}
}

if (-e $ARGV[0] and -d $ARGV[0]){
	find(\&countExt, $ARGV[0]);
	print "\n";
	my @extensions = keys %hash;
	@extensions = sort @extensions;
	foreach my $ext (@extensions){	#for each extension
		print $ext, ": ";
		@{$hash{$ext}} = sort @{$hash{$ext}};		# not @{$ext}!
		foreach (@{$hash{$ext}}){	#for each file having the extension
			print;			#print file name
			print " ";
		}
		print "\n";
	}
}
else {
	die "directory not exists";
}