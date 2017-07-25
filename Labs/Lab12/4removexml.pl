#!/usr/bin/perl
use warnings;
use strict;

open XML,"<".$ARGV[0];
open XMLout, ">".$ARGV[0]."_XMLremoved";

while(<XML>){
	s/\<\/?[a-zA-Z0-9_]+\ *([a-zA-Z0-9_:]+\ *= *\"[a-zA-Z0-9_\:\/\.\-]+\")*\>//g;
	print XMLout;
}