/* 
 * hide.c: 
 * Implementation of steganography on a binary ppm file (with magic number P6)
 * i.e. one bit of a message is hidden in the last bit of each color space 
 * i.e. storing one 8 bit character requires 8 total color spaces or 8/3 pixels.
 * 
 * This file contains both main function and the functions considered major
 * parts of the program. Other helper functions are in asg1fn.c.
 * 
 * This is a part of FIT3042 assignment1
 * 
 * Limitation: although, can check ppm of color depth up to 16 bits, always assume
 * color depth is 8 bits deep.
 * 
 * By: Pavin Wu (26916592)
 * Date: 14/4/17
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "asg1fn.h"

/* Function: writeHeader
 * ---------------------
 * write ppm header into fout
 * 
 * 
 */ 
void writeHeader (int width, int height, int color, FILE *fout) {
	
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
		 * write only len byte: to take away '\0'
		 */
	}
}

int writeMsg(char buffer, int width, int height, FILE *fin, FILE *fout) {
	// Take & write message
	float maxChar = (float) (width*height)*3/8;	
		// max possible number of characters, deduced from width & height in header 
		/* Takes 8/3 pixels to store 1 character.
		 * So x pixels can store x*3/8 characters.
		 * */
	int maxChar_int = (int) maxChar;			
		/* Can store at most integer number of actual characters.
		 * Need to round maxChar down by cast to int.
		 * */
	char *msg = (char *) malloc(sizeof(char)*maxChar_int);
	if (msg==NULL) {
		return 3;
	}
	printf("Type in secret message:\n");
	size_t msgCount = fread(msg, 1, maxChar_int-1, stdin); 
		/* fread with size maxChar_int-1. Take away 1 for '\0';
		 * maxChar_int-1 as nmemb instead of size since we want to "count" value
		 * with size 1 */
	*(msg+msgCount) = '\0';	//add string terminating character
	int binary[8] = {0,0,0,0,0,0,0,0};
	int j, i;	//can support msg of 2^31/(8*3) total characters (or strlen(msg) will exceed)
	for (i=0; i<strlen(msg)+1; i++){	//for each character from msg (include '\0')
		toBinary(msg[i], binary, 8);	 //8 bits binary of one character
		for (j=0; j<8; j++){			 //embed each of these bits to 8bytes from ppm 
			/* change value of buffer by one => equiv. to change last bit
			 * change only if last bit is different to binary[j] */
			if (buffer%2==0 && binary[j]==1) {	
				buffer+=1;
			}
			else if ((buffer%2==1||buffer%2==-1) && binary[j]==0){
				buffer-=1;
			}
			fwrite(&buffer, 1, 1, fout);	//write new byte value
			if (!feof(fin)){				//read new byte
				fread(&buffer, 1, 1, fin);
			}
			else {
				printf("PPM header and actual size unmatched. File truncated.\n");
				j=8; i=strlen(msg)+8;			//break out of loops
			}
		}
	}
	if (fread(msg, 1, 1, stdin) != 0){
		printf("PPM file is not large enough to contain the whole message. ");
		printf("Message truncated with first %lu characters remained", msgCount);
	}
	while (!feof(fin)) {
		fwrite(&buffer, 1, 1, fout);
		fread(&buffer, 1, 1, fin);
	}
	
	free(msg);
	return 0;
}

int main(int argc, char *argv[]) {
	
	// Check arguments
	int err = checkArg(argc, argv, 3);
	if (err) {return awMan(err);} 
	
	// Read file in binary format
	FILE *fin = fopen(argv[1], "rb");
	if (fin == NULL) {return awMan(4);}
	
	// Check headers
	int width, height, color;
	char buffer;			//also to be used later
	err = headerChecking(fin, &width, &height, &color, &buffer);	
		/* buffer now contains value of first color, first pixel from 
		 * ppm's binary section*/
	if (err) {return awMan(err);}
	printf("No errors in header\n");
	
	// Write header to output in binary format
	FILE *fout = fopen(argv[2], "wb");
	if (fout == NULL) {return awMan(4);}
	writeHeader(width, height, color, fout);
	
	err = writeMsg(buffer, width, height, fin, fout);
	if (err) {return awMan(err);}
	//buffer is from headerChecking above
	
	//check free too!? (????)

	fclose(fin);
	fclose(fout);

	return 0;
}
