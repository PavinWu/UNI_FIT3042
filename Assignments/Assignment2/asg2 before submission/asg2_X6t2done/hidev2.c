/* 
 * hide.c: 
 * Implementation of steganography on multiple binary ppm files (magic number P6)
 * i.e. one bit of a message is hidden in the last bit of each color channel 
 * i.e. storing one 8 bit character requires 8 total channels or 8/3 pixels.
 * 
 * This file contains both main function and the functions considered major
 * parts of the program. Other helper functions are in asg1fn.c.
 * 
 * This is a part of FIT3042 assignment2
 * 
 * Limitation: although, can check ppm of color depth up to 16 bits, 
 * always assume color depth is 8 bits deep, and will encode message accordingly
 * 
 * By: Pavin Wu (26916592)
 * Date: 24/4/17
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "asg1fn.h"
#include "asg2fn.h"

/* Function: writeHeader
 * ---------------------
 * write ppm header into fout
 * 
 * width: integer width of desired ppm image
 * height: integer height of desired ppm image
 * color: integer color depth of desired ppm image (between 1 to 2^16-1)
 * fout: file pointer to write the header to
 * 
 * return integer 4 if file open error, otherwise return 0.
 * Will always return 0 for the implementation in hide.c as fout is checked
 * before hand (just to be explicit).
 */ 
int writeHeader (int width, int height, int color, FILE *fout) {
	
	if (fout == NULL) {return 4;}
	
	char headerBuff[7] = {'P', '6', '\n', '\0'};	
		/* buffer for header contents.
		 * size of 7: 
		 * 		likely max size is 5 digits number => array size 5 +
		 * 		1 for null terminating character '\0' + 
		 * 		1 for '\n' to indicate the end of current header content.
		 */
	int len, i;
	for(i=0; i<4; i++) {
		switch (i) {
			case 0:
				break;
			case 1:
				sprintf(headerBuff, "%i\n", width);	
				// print string of integer value width to headerBuff
				break;
			case 2:
				sprintf(headerBuff, "%i\n", height);
				break;
			case 3:
				sprintf(headerBuff, "%i\n", color);
				break;
		}
		len = strlen(headerBuff);
		fwrite(&headerBuff, len, 1, fout);	
		/* write content in headerBuff to fout.
		 * write only len byte: to take away '\0'. (Header not string)
		 */
	}
	return 0;
}

/* Function: changeLastBit
 * -----------------------
 * Change last bit of an 8bit character to the binary value.
 * 
 * buffer: pointer to the 8bit character whose last bit is to be changed
 * binary: value of buffer's new last bit 
 * 
 * return integer -1 if buffer points to NULL, otherwise returns 0.
 * Will not encounter this with the implementation in hide.c as *buffer was 
 * given a value after a successful headerChecking.
 */
int changeLastBit (char *buffer, int binary){
	/* change value of buffer by one => equiv. to change last bit
	 * change only if last bit is different to binary[lastEmbedBin] */
	if (buffer==NULL) {return -1;}
	if (*buffer%2==0 && binary==1) {	
		*buffer+=1;
	}
	else if ((*buffer%2==1||*buffer%2==-1) && binary==0){
		/* check -1 too as *buffer is signed */
		*buffer-=1;
	}
	return 0;
}

/* Function: embedBinary
 * -----------------------
 * embed each of 8bit binary into 8bytes from ppm
 * 
 * buffer: pointer to buffer of a single byte from ppm file. 
 * 	 	If not EOF first, will be changed after the first iteration of the loop.
 * binary: integer array containing the bits to embed
 * fin: pointer to ppm file to be read
 * fout: pointer to output ppm file
 * lastEmbedBin: a pointer to loop counter, used to specify the bit in binary
 * 		to be written. Will be non-0 if previous file terminates before all 8
 * 		bits were written
 * 
 * return integer 0 if there is no error, 
 * 				10 if ppm is truncated.
 * 
 * NOTE: Please check fin and fout not empty yourself!
 */
int embedBinary(char *buffer, int *binary, FILE *fin, FILE *fout, int *lastEmbedBin){
	for (; (*lastEmbedBin)<8; (*lastEmbedBin)++){	 		
		changeLastBit(buffer, *(binary+(*lastEmbedBin)));
		fwrite(buffer, 1, 1, fout);		//write new byte value
		if (!feof(fin)){				
			fread(buffer, 1, 1, fin);		//read new byte
		}
		else {			//if end of file
			return 10;
		}
	}
	return 0;
}

/* Function: headerCheckingAll
 * ---------------------------
 * Check header for all [picnum] number of ppm.
 * 
 * width: pointer to int (array) to store width data of each ppm file obtained
 * 		during headerChecking. fin[i] will have width[i].
 * height: same as width, but for height.
 * color: same as width, but for color.
 * buffer: same as width, but of type char. We have this as we only know when
 * 		header terminates by reading until first binary bit of ppm (?). But this
 * 		first bit is a part of data we want, so we need to pass this on.
 * **WARNING** width, height, color and buffer needs to have the size of [picnum], 
 * 		or will cause memory overrun.
 * 
 * picnum: number of ppm picture to have header checked. If input has no flag, 
 *		use picnum==1. If has -m flag, input picnum the number-of-files.
 *		Other flags essentially behave as no flag for each ppm.
 * totSize: total possible message size that can be stored in ppms. Includes \0.
 * fin: array of file-pointers, for which the opened file with in the function
 * 		can later be used else where in the program
 * finname: starting filename whose header to be checked. If -m flag, will
 * 		be in format [filename]-000.ppm. Other flags will be [filename].ppm
 * opmode: operating mode for program. For meaning of each opmode, see the 
 * 		single digit flags on checkArg2 in asg2fn.c
 * 
 * return 0 if no errors, 4 if fin[i] is NULL, or error of headerChecking (from
 * 		asg1fn).
 */
int headerCheckingAll(int *width, int *height, int *color, char *buffer, 
			int picnum, int *totSize, FILE *fin[], char *finname, int opmode){
	
	*totSize = 0;
	
	int i, err;
	for(i=0; i<picnum; i++){
		fin[i] = fopen(finname, "rb");
		if (fin[i] == NULL) {
			if (i>0) {
				int k;
				for(k=0; k<i; k++){
					fclose(fin[k]);
				}
			}
			return 4;	
		}
		
		err = headerChecking(fin[i], (width+i), (height+i), (color+i), (buffer+i));	
		/* *(buffer+i) now contains value of first color, first pixel from 
		 * ppm's binary section*/
		if (err) {return err;}
		
		*totSize += (*(width+i))*(*(height+i));
		if (opmode == 1) {changeNameNum(finname, i);}
	}
	*totSize = ((*totSize)*3)/8;
	/* To store 1 character in ppm P6 image with color depth of 8bit, we need 
	 * 8/3 pixels i.e. 3/8 character per 1 pixel. So x pixels can store x*3/8 
	 * characters. */
	return 0;			
}


/* Function: readMsg
 * -----------------
 * read secret message from stdin, to be written in ppm
 * 
 * width: width information obtained from original ppm's header
 * height: height information obtained from original ppm's header
 * msg: pointer to the message to be read.
 * msgLen: pointer to size_t of the message written (+1 if need to include '\0'.
 * 		This "message" is what was read from fread. May be less than what's 
 * 		actually input through stdin!
 * 
 * return integer 0 if no error, 3 if malloc returns NULL pointer,
 */
 /*
 * NOTES: 
 *  - Do fread with maxChar_int-1 to as 
 * 		- maxChar_int is the only info we may deduce initially. If the ppm image
 * 		  is truncated, we can deal with it as we go.
 * 		- -1 to keep space for '\0'. Just so we can deal with the msg like a string
 *  - Put maxChar_int-1 as nmemb argument instead of size argument, since we 
 * 	  want to "count" based on the assumption that a unit value is of size 1.
 *	  This allows us to append '\0' successfully at the end. 	
 *  - Can support write message of upto 2^31/(8*3) total characters (or the integer
 *    counter will overflow and will not be able to keep track of the message.
 *    It is unlikely that user will provide a message that big, however (guess).
 */
int readMsg(int maxChar_int, char *msg, size_t *msgLen) {
	
	if (msg==NULL || msgLen==NULL) {return 3;}
	printf("Type in secret message: \n");
	
	*msgLen = fread(msg, 1, maxChar_int-1, stdin); 	
	*(msg+(*msgLen)) = '\0';	//add string terminating character

	return 0;
}

/* Function: writeMsg
 * ------------------
 * Write the secret message onto a copy of desired ppm. 
 * Similar to writeMsg for asg1, but this one allows not writing whole message
 * to one ppm file. Also allow to keep track of where message was last written.
 * These are done by passing values of pointers to next instance of writeMsg2,
 * had fin terminates before all bits in binary were written.
 * (This function is for one ppm only. If need to do with multiple ppms, 
 * use for loop with this function.) 
 * 
 * buffer: starting character to write into binary section of ppm
 * msg: char pointer to message to be written.
 * msgLen: size_t struct indicating the size of message to be written (excluding
 * 		'\0')
 * fin: pointer to original ppm file
 * fout: pointer to ppm file to write to
 * counter: pointer to integer indicating how much of msg is written.
 * lastEmbedBin: pointer to integer indicating the last index of binary array,
 * 		which its binary value was already written.
 * binary: binary array to write onto ppm.
 * 
 * return 0 if no error, 4 if file open error, else err of embedBinary().
 */
int writeMsg2(char *buffer, char *msg, size_t *msgLen, FILE *fin, FILE *fout,
				int *counter, int *lastEmbedBin, int *binary) {

	if (fin==NULL || fout==NULL) {return 4;}
	int eof;
	
	//while message not end
	for (; ((int) *counter)<((int) (*msgLen)+1); (*counter)++){
		//previous write complete (all bits got in before file's EOF)
		if (*lastEmbedBin >= 7){	
			*lastEmbedBin = 0;
			toBinary(msg[*counter], binary, 8);	 //get 8 bit binary of one char
		}
		//previous write incomplete
		else {(*lastEmbedBin)++;} //don't want to write last written bit again
		
		eof = embedBinary(buffer, binary, fin, fout, lastEmbedBin);
		if (eof) {		//end of file
			// got last lastEmbedBin of binary written
			// got binary
			// got last *counter of message (if not complete write, 
			// 		continue with same character i.e. *counter, just write
			//		lastEmbedBin < 7.)
			return eof;
		}
	}
	
	//if msg ends but not end of file
	while (!feof(fin)) {
		fwrite(&buffer, 1, 1, fout);
		fread(&buffer, 1, 1, fin);
	}
	
	return 0;
}


/* Function: makeNameop1
 * ---------------------
 * make starting input, output name if operation mode of program is 1 (flag -m).
 * i.e. append '-000.ppm\0' at the end.
 * 
 * finname: char pointer with [size of file name from argv] + 9.
 * 		The size of memory pointed to finname must be large enough for 
 * 		additional '-000.ppm\0' (9 additional characters).
 * foutname: same as finname, but for output file
 * argv: arguments containing initial finname, foutname to be appended
 * 		(without '-000.ppm\0').
 * 
 * return 0 if no error, 1 if arguments incomplete for operation mode 1.
 */
int makeNameop1(char *finname, char *foutname, char *argv[]){
		char postfix[9] = {'-','0','0','0','.','p','p','m','\0'};
		if (argv[2]==NULL || argv[3]==NULL || argv[4]==NULL){
			printf("Not enough arguments\n");
			return 1;
		}
		strcpy(finname, argv[3]);
		strcat(finname, postfix);
		
		strcpy(foutname, argv[4]);
		strcat(foutname, postfix);
		
		return 0;
}

/* FUNCTION: writeAllMsg
 * ---------------------
 * write message each file per loop inside the function.
 * 
 * buffer: buffer of a byte from ppm
 * msg: message to be written
 * msgLen: length of message to be written
 * fin: an array of pointer to ppm file to read
 * foutname: file name of fout
 * picnum: number of file to read/write
 * opmode: operation mode of program depending on the flag
 * width: a pointer to array of width of each image
 * height: same as width, but for height
 * color: same as width, but for color
 * 
 * return 0 if no error, 4 if fout cannot open foutname. Otherwise return error
 * 		of writeHeader (if any)
 */
int writeAllMsg(char *buffer, char *msg, size_t *msgLen, FILE *fin[], 
		char *foutname, int picnum, int opmode, int *width, int *height, int *color) {

	FILE *fout = NULL;
	int *lastEmbedBin = (int *) malloc(sizeof(int)); 
	int *counter = (int *) malloc(sizeof(int));	
	*lastEmbedBin = 7;	//start as if it is a previous successful write
	*counter = 0; //Counter is current message written on file
	int binary[8] = {0,0,0,0,0,0,0,0};
	
	int i, err = 0;		//initialize helper variables
	for(i=0; i<picnum; i++){
		/** open output file **/
		fout = fopen(foutname, "wb");
		if (fout == NULL) {
			err = 4;
			goto CLEARwriteAllMsg;
		}
		
		/** write header **/
		err = writeHeader(*(width+i), *(height+i), *(color+i), fout);
		if (err) {
			goto CLEARwriteAllMsg;
		}

		/** write message **/
		writeMsg2(buffer, msg, msgLen, fin[i], fout, counter, lastEmbedBin, binary);
		// don't catch EOF return error here! (since it is expected) 
		
		if (opmode == 1){
			changeNameNum(foutname, i+1); 	// increment file name
		}
		fclose(fout);
		fout = NULL;
	}
	
	CLEARwriteAllMsg:
		for(i=0; i<picnum; i++){
			fclose(fin[i]);
		}
		if (fout != NULL) {fclose(fout);}
		free(lastEmbedBin);
		free(counter);
		
	return err;	
}

/* Function: readWriteAllMsg
 * -------------------------
 * Read header of each file, for a total of [picnum] file.
 * Then read the message to write onto the copies of those files.
 * Then write the message.
 * 
 * finname: char pointer with [size of file name from argv] + 9.
 * 		The size of memory pointed to finname must be large enough for 
 * 		additional '-000.ppm\0' (9 additional characters).
 * foutname: same as finname, but for output file
 * picnum: integer number of total number of ppm file to read and write
 * totSize: max number of characters the message can have
 * 
 * return 0 if no error, 3 if cannot allocate space for some variables. 
 * 		Otherwise, return the error of headerCheckingAll, readMsg.
 */
int readWriteAllMsg (char *finname, char *foutname, int picnum, int opmode, int *totSize){
	
	/** Prepare reading & writing message**/
	int *width = (int *) malloc(sizeof(int)*picnum);
	int *height = (int *) malloc(sizeof(int)*picnum);
	int *color = (int *) malloc(sizeof(int)*picnum);
	char *buffer = (char *) malloc(sizeof(char)*picnum);
	char *msg = NULL; 
	size_t *msgLen = NULL;
	
	int err = 0;
	if(width==NULL || height==NULL || color==NULL || buffer==NULL){
		err = 3;
		goto CLEARreadWriteAllMsg;
	}
	
	{//sub-branch: to avoid error when use *fin[] with goto
	/** get header, and hence possible message size (totSize) **/
	FILE *fin[picnum]; 		//declare array of pointers to different fin
	err = headerCheckingAll(width, height, color, buffer, picnum, totSize, 
			fin, finname, opmode);
	if (err) {goto CLEARreadWriteAllMsg;}
		
	/** read message **/
	msg = (char *) malloc(sizeof(char)*(*totSize));
	msgLen = (size_t *) malloc(sizeof(size_t));	 //no. of char read
	if (msg==NULL || msgLen==NULL) {
		err = 3;
		goto CLEARreadWriteAllMsg;
	}
	err = readMsg(*totSize, msg, msgLen);
	if (err) {goto CLEARreadWriteAllMsg;}
		
	/** write message **/
	err = writeAllMsg(buffer, msg, msgLen, fin, foutname, picnum, 
				opmode, width, height, color);
	
	/** check if image(s) stored all messages **/
	if (!(feof(stdin))){	//if still msg after ppm run out of space
		printf("\n-PPM file is not large enough to contain the whole message.");
		printf(" Message truncated.-");
		// only check stdin: already limitted msg size when do fread
	}
	}

	CLEARreadWriteAllMsg:
		free(width); free(height); free(color); free(buffer);
		free(msg); free(msgLen);
		return err;
}

/* Function: forkAndExec
 * ---------------------
 * fork and execute a program with parameter params
 * 
 * params: array of pointers to the parameter to pass on to the program with
 * 		name params[0].
 * 
 * return status with which child process exits.
 */
int forkAndExec(char *params[]){
	int status;
	pid_t pid = fork();
	if (pid){
		waitpid(pid, &status, 0);
	}
	else {
		execv(params[0], params);
	}
	return status;
}
	
/* Function: readTxtCommand
 * ------------------------
 * Read text file to determine message, name of input file, name of output file
 * Then fork and execute ./hide based on the input file, output file and message
 * one-by-one.
 * 
 * argv2: char pointer containing the name of the text file to read command.
 * nameLenLimit: max length the name the message, name of input file and name of
 * 		output file can have.
 * 
 * return 0 if no error, 1 filename exceed nameLenLimit, 4 if cannot allocate 
 * 		space for variable.
 */
int readTxtCommand(char *argv2, int nameLenLimit){
	 FILE *ftxt = fopen(argv2, "r");	//text file containing operations
 	 if (ftxt==NULL) {return 4;}
 	 
 	 int err = 0; char buffer;
 	 /** initialize parameters to be passed to child process**/
	 char *params[5];
	 params[0] = (char *) malloc(7*sizeof(char));
	 params[1] = NULL; params[2] = NULL; params[3] = NULL;	//initialize
	 params[4] = 0;	 //self-note: want null pointer, not just value 0 somewhere!
	 if (params[0]==NULL) {
		 err=4;
		 goto CLEARop2Exec;
	 } 
	 strcpy(params[0], "./hide");
	 
	 /** create buffer for a parameter **/
	 char *paramBuff = (char *) malloc(nameLenLimit*sizeof(char));	//initialize
	 if (paramBuff == NULL) {
		 err=4;
		 goto CLEARop2Exec;
	 }
	 
	 /** Go through text file. Fork and Exec child process after '\n' **/
	 int i=0; int j=3; //i counts word's character, j counts param num
	 while(!feof(ftxt)){	//save one position for '\0'/
		 fread(&buffer, 1, 1, ftxt);
		 
		 if(isspace(buffer)){
			 /* put paramBuff to params[j] after whitespace */
			 *(paramBuff+i) = '\0';
			 params[j] = (char *) malloc((strlen(paramBuff)+1)*sizeof(char));
			 if (params[j] == NULL){
				 err = 4;
				 goto CLEARop2Exec;
		     }
			 strcpy(params[j], paramBuff);
			 
			 if (j==3) j=1;
			 else if (j==1) j=2;
			 else j=3;
			 i = -1;	//to be ++ next loop
		 }
		 else {
			 /* append buffer to paramBuff if normal text */
			 *(paramBuff+i) = buffer;
			 if(i>=nameLenLimit-1){
				 printf("Filenames must be of length less than %d\n", nameLenLimit);	/////////////////////////////LIMITATION
				 err = 1;
				 goto CLEARop2Exec;
			 } 
		 }
		 
		 /* fork and execute */
		 if(buffer=='\n'){
			 if (params[1]!=NULL && params[2]!=NULL && params[3]!=NULL){
				 forkAndExec(params);
				 free(params[1]); free(params[2]); free(params[3]);
				 params[1] = NULL; params[2] = NULL; params[3] = NULL;
				 //avoid params to have unintended values
			 }
		 }
		 i++;
	 }
	 
	 CLEARop2Exec:
		 fclose(ftxt);
		 free(paramBuff);
		 for (i=0; i<5; i++){
			 free(params[i]);
		 }
		 return err;
}

/* MAIN for hidev2.c */
int main(int argc, char *argv[]) {
	/** Initialize variables **/
	int err = 0;	//
	int picnum;
	char *finname = NULL; 
	char *foutname = NULL;	//
	int *totSize = (int *) malloc(sizeof(int));

	if (totSize==NULL) {
		err = 3;
		goto CLEARALL;
	}
	
	/** Get operating mode depending on flag **/
	if (argc < 2) {
		printf("Need at least 1 arguments.\n");
		err = 1;
		goto CLEARALL;
	}
	int opmode = checkFlag(argv[1]);
	if (opmode < 0) {
		err = 11;
		goto CLEARALL;
	}
	
	/** check arguments based on opmode **/
	err = checkArg2(opmode, argv);
	if (err) {goto CLEARALL;}
	
	/** get name of fin & fout based on opmode **/
	if (opmode == 1) {
		finname = (char *) malloc(sizeof(char)*(strlen(argv[3])+9));
		foutname = (char *) malloc(sizeof(char)*(strlen(argv[4])+9));
		if (finname == NULL || foutname == NULL) {
			err = 3;
			goto CLEARALL;
		}
		picnum = atoi(argv[2]);
		err = makeNameop1(finname, foutname, argv);
	}
	else if (opmode == 2){
		err = readTxtCommand(argv[2], 100);	//execute the operations here
		goto CLEARALL;
	}
	else if (opmode == 0 || opmode == 3){
		/* compare argv[2] and argv[3] -> output error if equal
		 * then go like normal with argv[2] as finname, fout as foutname
		 * (like with opmode==0) */
		picnum = 1;
		
		int addNameNum = 0;
		if (opmode == 3) {addNameNum++;}
		finname = strdup(argv[1+addNameNum]);
		foutname = strdup(argv[2+addNameNum]);
	}
	
	if (finname==NULL || foutname==NULL) {
		err = 3;
	}
	if (err) {goto CLEARALL;}
	
	if (opmode == 0 && argc>3){	   //if get msg from file instead of stdin
		if (argv[3] == NULL) {
			err = 1;
			goto CLEARALL;
		}
		FILE *f_all = fopen(argv[3], "r");
		if (f_all == NULL) {
			err = 4;
			goto CLEARALL;
		}
		dup2(fileno(f_all), STDIN_FILENO);
	}
		
	/** Read & Write message **/
    err = readWriteAllMsg (finname, foutname, picnum, opmode, totSize);
		
	if (err) {goto CLEARALL;}
	if (opmode==0 && argc>3){	// and no error with file opening (or will not get here)
		fclose(stdin);
	}

	/** clear all allocated memories **/
	CLEARALL:
		free(finname); free(foutname);
		free(totSize);
		if (opmode != 2) return awMan(err);	//including 0
		else {
			if(err) return awMan(err); //don't print when parent, if no error
		}
		return 0;
}
