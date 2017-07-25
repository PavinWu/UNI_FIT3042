dear and undear: dedupping arhiever and undedupping unarchiver
	delete_dup.pl is for dear and restore_dup.pl is for undear.
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
	Archive a directory into a tar, but with duplicate files removed.
	May choose to compress this further with:
		gzip (specify with -g flag),
		bzips (specify with -b flag),
		compress (specify with -c flag).
	You may also choose not to apply a compression by not specifying a flag.

	Execute this just as you would with any executable i.e. 
		0th argument as a directory to program, including the program name itself.
	1st argument is used to specify flag for compression option. You may leave this
		blank if you want to leave arhieve file in tar format.
	2nd argument is used to specify arhieved output file. Do not end it with file extension
		unless you want it as a part of the name. File extension automatically appended
		during arhieving process.
	3rd argument is used to specify the source directory to be arhieved.
	Example: compress indir into outfile.tar.Z in current directory:
		./dear -c outfile indir

	The arhieve will contain a version of the directory with duplicate files removed, 
		while keeping a single copy of duplicates in the original directory
	It will also contain a text file specifying duplicate files found by fdupes.
		This may be used to restore the duplicates in undear.
		
	NOTE: this program will output some tmp paths and others when running. 
		This is due to the program using pushd and popd. Don't panic!

undear: undedupping unarhiever
------------------------------
	Unarchieve a tar in current directory, with duplicate files either:
		deleted (specify with -d flag),
		restored as a (absolute) symlink to the remaining copy of duplicates (specify with -l flag),
		restored as a copy of the remaining copy of duplicates (specify with -c flag).
	You must specify a flag.

	Execute this just as you would with any executable i.e. 
		0th argument as a directory to program, followed by the program name itself.
	1st argument is used to specify flag.
	2nd argument is used to specify the arhieve file to unarhieve.
	Example: uncompress outfile.tar.Z to current directory with all copies restored:
		./undear -c outfile.tar.Z
	
Known Bugs & Limitations
------------------------
	dear: -e[en]* and -n[en]* flags are interpretted as a regular file name (not as a flag). (??????)
		Since <outfile> is interpretted as a <indir>, will get "ERROR: <outfile> not a directory" if
		there is no directory with name <outfile>.
		In case <outfile> turns out to be a directory, will arhieve <outfile> directory into output 
		with name -e[en]*.tar or -n[en]*.tar in current directory, just like the case with no flag,
		and <indir> will be ignored.
	dear: the program uses /tmp to make a copy of the directory. Then DIR with the duplicates removed 
		will be archieved in the specified directory. This means an extra space of a little more than 
		twice that of original dir is needed. Operation will fail if there is not enough space in /tmp.
	undear: same as dear above, but uses /tmp to uncompress & untar the archived file. This is so the original 
		archived file is not altered. The restoration of duplicates are done in current directory, however.
	undear: if you choose to use symlink, be reminded that manipulating the directories (e.g. renaming)
		could break the link. Oddly enough, in test machine (Xubuntu 16.04), moving the whole directory 
		or just move a symlink, does not break the link. Renaming the directory or move the original file
		cause a link to break as expected.
		
	