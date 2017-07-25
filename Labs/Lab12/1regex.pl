#!/usr/bin/perl -w

#$text = chomp(<STDIN>);
#chomp ($text = <STDIN>);

print "Insert string to check: ";
while (<STDIN>){
	chomp;
	if ($_ eq "q"){
		die("exitting due to q input");
	}
	else {
		#if ($text =~ /[0-9]???\-[0-9]???/){
		if (/^[0-9]{4}\-[0-9]{4}$/){
			print "a) Phone number matched\n";
		}
		elsif (/^(((Mon|Tues|Wednes|Thurs|Fri|Satur|Sun)(day)?)|(Tue|Wed|Thu|Sat))$/i){
			# what happens if leave brackets?
			# evaluate ^ and $ before outermost |. Unless put brackets.
			# | evaluates last
			print "b) days of week matched\n";
		}
		#elsif (/^((([01]?[0-9]{1,2})|(2[0-4][0-9])|(25[0-5]))\.)\1{3}$/){
		#elsif (/^(([01]?[0-9]{1,2})|(2[0-4][0-9])|(25[0-5]))(\.)\1\2$/){	#nested grouping
		elsif (/^(([01]?[0-9]{1,2}|2[0-4][0-9]|25[0-5])(\.)){3}([01]?[0-9]{1,2}|2[0-4][0-9]|25[0-5])$/){	#back ref matches same string!
			#elsif (/^([0-255]\.){4}$/){
			# can only match to one digit
			# brackets with \. to separate it from 25[0-5] set
			# how to repeat the last number group with the earlier group?
			print "c) Dotted IPv4 address matched\n";
		}
		elsif (/^https*\:\/\/www\.[a-zA-Z0-9]+(\.[a-zA-Z0-9]+)+/){
			print "d) Absolute web address matched\n";
		}
		elsif (/^[A-Z][a-zA-Z0-9]*([:;,]? [a-zA-Z0-9]+)+[!.?]$/){
			# what if we put space in [a-zA-Z0-9] above?
			print "e) Looks like proper English sentence matched\n";
		}
		else {
			print "No matches\n";
		}
	}
	print "Insert string to check: ";
}