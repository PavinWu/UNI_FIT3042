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
 * Date: 15/4/17
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
	 * change only if last bit is different to binary[j] */
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
 * 
 * return integer 0 if there is no error, 
 * 				10 if ppm is truncated.
 * 
 * NOTE:
 * 		Please check fin and fout are not empty yourself.  
 */
int embedBinary(char *buffer, int *binary, FILE *fin, FILE *fout, int *j){
	for (; (*j)<8; (*j)++){	 		
		changeLastBit(buffer, *(binary+(*j)));
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

/* Function: readMsg
 * -----------------
 * read secret message from stdin, to be written in ppm
 * 
 * width: width information obtained from original ppm's header
 * height: height information obtained from original ppm's header
 * msg: pointer to the message to be read. Do not allocate before hand.
 * 		will be written over in this function
 * msgLen: pointer to size_t of the message written (need to +1 for 
 * 		actual message len with '\0'.
 * 		This "message" is what was read from fread. May be more than what's 
 * 		actually input through stdin!
 * 
 * return integer 0 if no error, 3 if malloc returns NULL pointer,
 */
 /*
 * NOTES: 
 * 	- To store 1 character in ppm P6 image with color depth of 8bit,
 * 	  we need 8/3 pixels i.e. 3/8 character per 1 pixel.
 *    So x pixels can store x*3/8 characters.
 *  - We still need to round x*3/8 down to integer, as writing only part of the
 *    8bit of the character does not allow us to fully interpret the character anyway.
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
	
	if (msg==NULL) {return 3;}
	printf("Type in secret message:\n");
	
	*msgLen = fread(msg, 1, maxChar_int-1, stdin); 
	*(msg+(*msgLen)) = '\0';	//add string terminating character

	return 0;
}

/* Function: writeMsg
 * ------------------
 * Write the secret message onto a copy of desired ppm.
 * Similar to writeMsg for asg1, but this one allows not writing whole message
 * to one ppm file. Also allow to keep track of where message was last written.
 * 
 * buffer: starting character to write into binary section of ppm
 * msg:
 * msgLen:
 * fin: pointer to original ppm file
 * fout: pointer to ppm file to write to
 * 
 * return integer 0 if no error, 4 if file open error, 
 * 			err of embedBinary() otherwise
 */
int writeMsg2(char *buffer, char *msg, size_t *msgLen, FILE *fin, FILE *fout,
				int *counter, int *j, int *binary) {

	if (fin==NULL || fout==NULL) {return 4;}
	int eof;
	
	//while message not end
	for (; ((int) *counter)<((int) (*msgLen)+1); (*counter)++){
		if (*j >= 7){	//previously complete write (all bits got in before EOF)
			*j = 0;
			toBinary(msg[*counter], binary, 8);	 //get 8 bit binary of single character
		}
		else {(*j)++;} //don't want to write last successful bit again
		eof = embedBinary(buffer, binary, fin, fout, j);
		if (eof) {		//end of file
			// got last j of binary written
			// got binary
			// got last *counter of message (if not complete write, continue with
			//		same character ie *counter)
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


/* Function: changeNameNum
 * ---------------------
 * change file name numbering to i
 * 
 * i is between 0 to 254 (can have between 1 to 255 ppm pictures)
 */
void changeNameNum(char *name, int i){
	int len = strlen(name);	//strlen == index of '\0'
	char num[4];
	
	if (i > 99){ 	//3 digits
		sprintf(num, "%i", i);
	}
	else if (i > 9){	//2 digits
		sprintf(num, "0%i", i);
	}	
	else {		//1 digit
		sprintf(num, "00%i", i);
	}
	
	*(name+len-5) = num[2];		//{..., '0','0','0','.','p','p','m','\0'}
	*(name+len-6) = num[1];
	*(name+len-7) = num[0];
}



/* MAIN for hidev2.c */
int main(int argc, char *argv[]) {
	// Initialize variables
	int opmode = -1; 
	int err, picnum;
	
	int *width, *height, *color; 
	char *buffer;
	
	char *finname = (char *) malloc(sizeof(char)*(strlen(argv[3])+9));
	char *foutname = (char *) malloc(sizeof(char)*(strlen(argv[4])+9));
	char postfix[9] = {'-','0','0','0','.','p','p','m','\0'};
	
	int i, totSize = 0;
	char *msg; size_t *msgLen;	//Len is total message size
	
	int binary[8] = {0,0,0,0,0,0,0,0};
	int *j = (int *) malloc(sizeof(int)); 
	int *counter = (int *) malloc(sizeof(int));	
	*j = 7;	//start as if it is a previous successful write
	*counter = 0; //Counter is current message written on file
	
	if (finname==NULL ||foutname==NULL || j==NULL) {
		return awMan(3);
	}
	
	// get operating mode
	opmode = checkFlag(argv[1]);
	if (opmode < 0) {return 11;}
	// Check arguments
	err = checkArg2(opmode, argv);
	if (err) {return awMan(err);}

	
	/***************************/
	/* THIS STILL NEEDs LOOKING */
	// Read file in binary format
	// need to read multiple times if opMode 1
	// need to read multiple times frmo file after read in file if opMode 2
	if (opmode==0 || opmode==3){
		picnum = 1;
	}
	else {//if (opmode==1){
		picnum = atoi(argv[2]);
	}
	/*else if (opmode==2){
		// read text file first before getting pic num
		// or may be we don't need picnum, but rather an array of file names
		// or may be still need picnum to loop
		// OR NOT: NEED TO USE FORK!!! ("parallel")
		// will need to separate the over lines from child process somehow
		// 		or may be not
	}*/
	/***************************/
	
	///////////////////////// for opmode 1 (-m flag) ///////////////////////////
	
	//////* check message size by checking headers *//////
	// Prepare fin
	width = (int *) malloc(sizeof(int)*picnum);
	height = (int *) malloc(sizeof(int)*picnum);
	color = (int *) malloc(sizeof(int)*picnum);
	buffer = (char *) malloc(sizeof(char)*picnum);
	
	if(width==NULL || height==NULL || color==NULL || buffer==NULL){
		return awMan(3);
	}
	
	FILE *fin[picnum]; //open array of pointers to different fin
	
	// make name
	finname = strdup(argv[3]);
	strcat(finname, postfix);
	
	// check header for each ppm
	for(i=0; i<picnum; i++){
		fin[i] = fopen(finname, "rb");
		if (fin[i] == NULL) {return awMan(4);}
		
		err = headerChecking(fin[i], (width+i), (height+i), (color+i), (buffer+i));	
		/* *(buffer+i) now contains value of first color, first pixel from 
		 * ppm's binary section*/
		if (err) {return awMan(err);}
		
		totSize += (*(width+i))*(*(height+i));
		//change name
		changeNameNum(finname, i);
	}
	printf("No errors in header.\n");
	
	
	//////* read message *//////
	totSize = (totSize*3)/8;
	msg = (char *) malloc(sizeof(char)*totSize);
	//if (msg==NULL) {return awMan(3);}
	msgLen = (size_t *) malloc(sizeof(size_t));	//no. of char read
	err = readMsg(totSize, msg, msgLen);
	if (err) {return awMan(err);}
	
	//////* write message *//////
	// Prep writing
	FILE *fout;
	foutname = strdup(argv[4]);
	strcat(foutname, postfix);
	
	// write message each file per loop
	for(i=0; i<picnum; i++){
		fout = fopen(foutname, "wb");
		if (fout == NULL) {return awMan(4);}
		
		// write header
		writeHeader(*(width+i), *(height+i), *(color+i), fout);
		
		// write message
		writeMsg2(buffer, msg, msgLen, fin[i], fout, counter, j, binary);
				
		changeNameNum(foutname, i+1); 	// increment file name
		fclose(fin[i]); fclose(fout);
	}
	
	if (!(feof(stdin))){	//if still msg after ppm run out of space
		printf("\nPPM file is not large enough to contain the whole message.\n");
		printf("Message truncated.\n");
	}
	
	
	
	free(msg); free(msgLen);
	free(width); free(height); free(color); free(buffer); 
	free(finname); free(foutname);
	free(j); free(counter);
		
		
	// op mode 1 (and 2?
	// can store it in array, or get message then read image one-by-one
	// and store accordingly.	

	
	/***************************/
	/*OLD CODE FROM ASG1*/
	// Check headers


	
	// Write header to output in binary format
	/*FILE *fout = fopen(argv[2], "wb");
	if (fout == NULL) {return awMan(4);}*/
	
	/*
	char *msg; size_t *msgCount;
	err = readMsg(width, height, msg, msgCount);
	if (err) {return awMan(err);}
	err = writeMsg(buffer, msg, msgCount, fin, fout);
	if (err) {return awMan(err);}
	//buffer is from headerChecking above
	*/

	/***************************/
	
	return 0;
}
