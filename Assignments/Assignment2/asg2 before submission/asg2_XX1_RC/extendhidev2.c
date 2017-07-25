/*
 * Helper functions for hidev2.c
 * 
 * By:Pavin Wu (26916592)
 * Date: 24/4/17
 * */

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
