Name:
------------------------
Pavin Wu (26916592)

Compile:
------------------------	
In terminal, change directory [cd] to where you store the extracted program files.
Compile the program by typing [make] (no brackets).

Run:
------------------------	
Change directory [cd] to where you store the compiled program.

To hide a message, type in terminal [./hide <ppm source file> <ppm output file>] where angle brackets
represent file name. If there were no errors with <ppm source file>, type in the message you wish to hide. 
You can type no more than what the size of <ppm source file> can allow, and you may do so no more than 500 characters
at a time. The output will be saved into <ppm output file>. 

To unhide the message, type in terminal [./unhide <ppm output file>]. if there were no errors with
<ppm output files>, the encoded message will be displayed on the terminal screen.


Functionality supported:
------------------------
- Encode message into a P6 ppm file with ./hide
- Decode message from a P6 ppm file with ./unhide
- File name validation
- Check if message's length fits into the input ppm file
- Input 500 characters at a time to keep char array buffer small. (Just to be safe, albeit a bit on the paranoid side) 
- Not support BPM.

Known bugs:
------------------------
None that I know of ...
However, have not tested:
- when number of message exceeds number of message the ppm can take in
- when image can take only less than 500 characters
- possibly others ...

Known limitations:
------------------------
- Will not check whether each message is actually of 500 characters. This means user will not be
able to input message if (total number of loop)*500>=maxChar_int, even though there may be places left.