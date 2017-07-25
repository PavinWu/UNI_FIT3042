Name:
------------------------
Pavin Wu (26916592)

Compile:
------------------------	
In terminal, change directory [cd] to where you store the extracted program files.
Compile the program by typing [make] (no brackets).
The user must have libsdl2 and libsdl2-dev installed into their system before compiling drawPPM.c.
	(which is a part of make command). Otherwise will have to compile by typing
	[make hide] and [make unhide] separately, but this means you will not be
	able to show ppm image (with ./hide -s).
Type the following lines onto terminal:
	[sudo apt-get install libsdl2-2.0] and [sudo apt-get install libsdl2-dev]
	Type password if prompted.

Run:
------------------------	
Change directory [cd] to where you store the compiled program.

To hide a single message:
- Into one file:
	type in terminal [./hide <ppm source file> <ppm output file>] where angle brackets
	represent file name. If there were no errors with <ppm source file>, type in the message you wish to hide. 
	Press ctrl+d to terminate input. The output will be saved into <ppm output file>. 
- Into multiple files:
	type in terminal [./hide -m <number-of-files> <ppm source files' base name> <ppm output files' base name>].
	<ppm source files' base name> and <ppm output files' base name> will be appended with
	'-000.ppm', '-001.ppm', '-002.ppm', ... as each file is read, depending on the
	number-of-files. Hence, the actual file must be named without the additional
	appened strings.
	However, the actual names of the source files must be named with '-000.ppm',
	'-001.ppm',... (depending on number-of-files) for the program to recognize the 
	files. If one file within the range does not exist, the program will return 
	error even if the existing image can carry the message you will enter.

To hide multiple messages in parallel, with each message per file:
	type in terminal [./hide -p <text file>], where text file is written in the format:
	[<txt1 containing 1st message to hide> <ppm source file1> <ppm output file1>
	 <txt2 containing 2nd message to hide> <ppm source file2> <ppm output file2>
	 ] (If need more than two messages, just do the same on the next line).
	Each specifier in angle brackets must be separated with a non line-feed whitespace,
	terminate the end of each input set with line-feed '\n'.
	
To show an input and an output image after hiding:
	type in terminal [./hide -s <ppm source file> <ppm output file>].
	The operation is the same as when inputting a single message onto a single file.
	Only that the messages of each file is displayed at the end for 10 seconds after
	rendering finished. The filename is indicated on the title bar of the image.

To unhide the message:
- One file at a time:
	type in terminal [./unhide <ppm output file>]. if there were no errors with
	<ppm output files>, the encoded message will be displayed on the terminal screen.
- Multiple files at a time:
	type in terminal [./unhide -m <ppm output files' base name>].
	You should do the same as in the case of [./hide -m ...], but for output files.

Functionality supported:
------------------------
- Encode message into a P6 ppm file with ./hide
- Decode message from a P6 ppm file with ./unhide
- File name validation
- Check if message's length fits into the input ppm file
- Encode and decode message with -m flag.
- Encode in parallel with -p flag.
- display images with -s flag.

Known bugs:
------------------------
- unhide: Will output -file open error- if embeded message does not end with '\0'
	even if file was opened successfully. This happens if you try to ./unhide
	a non-last file that was a part of -m operation. As far as I know, this does
	not happen in other cases.
- hide -p: need to press return after operation for prompt to show
- hide -p: output won't look neat if there were errors with some operations. Also cannot tell
	which output belongs to which child process.

Known limitations:
------------------------
- hide/unhide: file open requires the source folder to have already existed.
- hide/unhide: file name cannot contain space (which require " (quote) at start 
	and end of name. checkFileName() in asg1fn.c does not recognize importance of quote.)
- hide -p: text-message file name cannot contain space (commands are separated
	with space, and readTxtCommand cannot handle space within quote.
- hide -s: cannot check if libsdl2 is installed. If not, program will run twice
	but with no pictures shown.
- hide -s: can take very long to render images.

- hide: although, can check ppm of color depth up to 16 bits, always assume color depth 
	is 8 bits deep, and will encode message accordingly.
- hide -m: (?) hide does not give an error even if a file in range in missing.
- hide -p: text-message file-name length limited by value predefined in the function.
- hide: fork, exec: will not terminate if there were errors with fork() or exec().
- asg2fn: changeNameNum() will not return an error by itself if the numbering i is >999.
	This is checked with checkArg2().

- drawPPM: the images are not displayed "side-by-side", but rather on top of each other.
	The image's window can be moved, however.
