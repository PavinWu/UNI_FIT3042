#!/usr/bin/perl -w
use strict;

opendir DIR, ".";
my @dir = readdir DIR;
@dir = sort @dir;

foreach my $dir (@dir) {
	print $dir, "\n";
}
