dear and undear: dedupping arhiever and undedupping uncompressor
by Pavin Wu (26916592)
Written: 20 May 17 for FIT3042 Assignment 3

Setup
-----
First extract tar containing this README and script files into a directory of choice.
Then cd into the directory.
Then make the scripts executable by running the following command in bash:
	chmod +x dear delete_dup.pl undear restore_dup.pl
Then install fdupes to detect duplicate files by running the following (require root password):
	sudo apt-get install fdupes
If you'd like to compress/uncompress a .Z file, you need to run (require root password):
	sudo apt-get install ncompress
dear and undear are tested on xubuntu 16.04. No guarantee to how the program will
	perform in other systems. Also no guarantee for program's behaviour 
	if fdupes and ncompress are not installed.
As dear depends on delete_dup.pl to delete duplicates file, make sure to keep them in the
	same directory. This applies to undear and restore_dup.pl for a similar reason.

dear: dedupping arhiever
------------------------
Arhive a folder into a tar, but with duplicate files removed.
May choose to compress this further by specifying flag as first argument:
	-g for gzip, -b for bzip2, -c for Z (with compress)
You may also not specify any flag if you don't want a compression.

Execute this just as you would with any executable not under path i.e. 
	0th argument as a directory to program, followed by the program name itself.
1st argument is used to specify flag for compression option. You may leave this
	blank if you want to leave arhieve file in tar format.
2nd argument is used to specify arhieved output file. Do not end it with file extension
	unless you want it as a part of the name. File extension automatically appended
	during arhieving process.
3rd argument is used to specify the source folder to be arhieved.
Example: compress indir into outfile.tar.Z in current directory:
	./dear -c outfile indir

The arhieve will contain contain a version of the folder with duplicate files removed, 
	while keeping a single copy of duplicates in its original directory
It will also contain a text file specifying duplicate files found by fdupes.
	This may be used to restore the duplicates in undear.

undear: undedupping unarhiever
--------------------------------
Unarhieve a tar into a folder, with duplicate files either 
	deleted (specify with -d flag),
	restored as a symlink to the remaining copy of duplicates (specify with -l flag), 
	restored as a copy of the remaining copy of duplicates (specify with -c flag).
You must specify a flag.

Execute this just as you would with any executable not under path i.e. 
	0th argument as a directory to program, followed by the program name itself.
1st argument is used to specify flag.
2nd argument is used to specify the arhieve file to unarhieve.
Example: uncompress outfile.tar.Z to current directory with all copies restored:
	./undear -c outfile.tar.Z
	
Known Bugs & Limitations
------------------------
dear: -e[en]* and -n[en]* flag still got through despite being invalid. Will get 
	"ERROR: <outfile> not a directory" as -e and -n are interpretted not being a flag.
	If <outfile> turns out to be a directory (with same basename as outfile), will
	output archived <outfile> into the current directory with name -e[en]* or -n[en]*,
	just like the case with no flag.
