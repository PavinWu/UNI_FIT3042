#!/usr/bin/perl -w
use strict;
my $pi=0;
my $prev_pi=3;
my $d_count=0;
my $diff=3;
# can only increase with addition & start from 0 => only increment
while ($diff > 0.00001) {
	$prev_pi = $pi;
	if ($d_count%2==1) {
		$pi = $pi - 4/(2*$d_count+1);
	}
	else {
		$pi = $pi + 4/(2*$d_count+1);
	}
	$d_count++;
	if ($prev_pi > $pi) {
		$diff = $prev_pi - $pi;
	}
	else {
		$diff = $pi - $prev_pi;
	}
	print $pi." ".$diff."\n";
}
