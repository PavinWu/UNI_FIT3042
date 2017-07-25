#!/usr/bin/perl -w

sub median {
	my @num = @_;
	@num = sort(@num);
	my $mid_id = int(@num/2);	# @num in scalar context
	if (@num == 0){
		print "Input list empty\n";
	}
	if (@num % 2 == 1){
		return $num[$mid_id];
	}
	else {
		return ($num[$mid_id-1]+$num[$mid_id])/2;
	}
}

print (median (@ARGV),"\n");
