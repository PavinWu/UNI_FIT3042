/* 
 * hidev2.c: 
 * Implementation of steganography on multiple binary ppm files (magic number P6)
 * i.e. one bit of a message is hidden in the last bit of each color channel 
 * i.e. storing one 8 bit character requires 8 total channels or 8/3 pixels.
 * 
 * This file contains both main function and the functions considered major
 * parts of the program. Other helper functions are in extendhidev2.c.
 * Other functions which are shared between hide and unhide are in asg2fn.c 
 * and asg1fn.c.
 * 
 * This is a part of FIT3042 assignment2
 * 
 * By: Pavin Wu (26916592)
 * Date: 4/5/17
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
#include "extendhidev2.h"

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
 	 
 	 int status, err = 0; char buffer;
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
		 
		 /* If whitespace, put paramBuff to params[j] */
		 if(isspace(buffer)){
			 
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
		 /* If normal text, just append buffer to paramBuff */
		 else {
			 *(paramBuff+i) = buffer;
			 if(i>=nameLenLimit-1){
				 printf("Filenames must be of length less than %d\n", nameLenLimit);
				 err = 1;
				 goto CLEARop2Exec;
			 } 
		 }
		 
		 /** fork and execute **/
		 if(buffer=='\n'){
			 if (params[1]!=NULL && params[2]!=NULL && params[3]!=NULL){
				 
 				 pid_t pid = fork();
				 if (pid == 0){
					execv(params[0], params);
				 }
				 j = 3;		//in case there were errors (additional space) in
							//previous command set
				 free(params[1]); free(params[2]); free(params[3]);
				 params[1] = NULL; params[2] = NULL; params[3] = NULL;
				 //avoid params to have unintended values
			 }
		 }
		 i++;
	 }
	 
	 /** wait for child process to terminate **/
	 for (; i>0; i--) {
		 wait(&status);
	 }
	 
	 CLEARop2Exec:
		 fclose(ftxt);
		 free(paramBuff);
		 for (i=0; i<5; i++){free(params[i]);}
		 return err;
}

/* Function: drawPPMs
 * ------------------
 * draw ppms with name ppm1Name, ppm2Name out on the screen.
 * This is done by forking two separate processes and execute ./drawPPM with
 * each of argument ppm1Name and ppm2Name.
 * 
 * ppm1Name: filename of 1st PPM
 * ppm2Name: filename of 2nd PPM
 * 
 * return 0 always. 
 */
int drawPPMs(char *ppm1Name, char *ppm2Name){
	char *params[] = {"./drawPPM", ppm1Name, 0};
	
	int i=0;
	for (; i < 2; i++){
		pid_t pid = fork();
		if (pid == 0){
			execv(params[0], params);
		}
		params[1] = ppm2Name;
	}
	int status;
	for (;i>0;i--){
		wait(&status);
	}
	return 0;
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
	
	/** get name of fin & fout based on operating mode **/
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
		/* opmode==3: compare argv[2] and argv[3] -> output error if equal*/
		picnum = 1;
		
		int addNameNum = 0;
		if (opmode == 3) {addNameNum++;}
		finname = strdup(argv[1+addNameNum]);
		foutname = strdup(argv[2+addNameNum]);
	}
	
	if (finname==NULL || foutname==NULL) {
		err = 3;
		goto CLEARALL;
	}
	
	/** get message from file instead of stdin if argc>3 
	 * (i.e. if a child process called from parent process with -p flag */
	if (opmode == 0 && argc>3){
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

	if (opmode == 3){
		err = drawPPMs(finname, foutname);
		if (err) {goto CLEARALL;}
	}
	
	/** clear all allocated memories **/
	CLEARALL:
		free(finname); free(foutname);
		free(totSize);
		if (opmode != 2) {return awMan(err);}	//including 0
		else {
			if(err) {return awMan(err);} //don't print when parent, if no error
		}
	return 0;
}
