#!/usr/bin/perl

opendir (DIR, ".");
while ( defined ($filename = readdir(DIR))) {	#DO???
	print ($filename, "\n");
}
