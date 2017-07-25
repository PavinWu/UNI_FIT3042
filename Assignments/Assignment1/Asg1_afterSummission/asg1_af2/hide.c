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

/* Function: writeMsg
 * ------------------
 * Write the secret message onto a copy of desired ppm.
 * 
 * buffer: starting character to write into binary section of ppm
 * width: width information obtained from original ppm's header
 * height: height information obtained from original ppm's header
 * fin: pointer to original ppm file
 * fout: pointer to ppm file to write to
 * 
 * return integer 0 if no error, 3 if malloc returns NULL pointer,
 * 		4 if file open error.
 * 
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
 * 
 */
int writeMsg(char buffer, int width, int height, FILE *fin, FILE *fout) {
	// Take & write message
	float maxChar = (float) (width*height)*3/8;	
	int maxChar_int = (int) maxChar;			
	
	char *msg = (char *) malloc(sizeof(char)*maxChar_int);
	if (msg==NULL) {return 3;}
	if (fin==NULL || fout==NULL) {return 4;}
	printf("Type in secret message:\n");
	size_t msgCount = fread(msg, 1, maxChar_int-1, stdin); 
	*(msg+msgCount) = '\0';	//add string terminating character
	int binary[8] = {0,0,0,0,0,0,0,0};
	int j, i;
	for (i=0; i<strlen(msg)+1; i++){	//for each character from msg (include '\0')
		toBinary(msg[i], binary, 8);	 //get 8 bit binary of single character
		for (j=0; j<8; j++){			 //embed each of these bits to 8bytes from ppm 
			
			changeLastBit(&buffer, binary[j]);
			fwrite(&buffer, 1, 1, fout);	//write new byte value
			if (!feof(fin)){				//read new byte
				fread(&buffer, 1, 1, fin);
			}
			else {
				printf("\nPPM header and actual size unmatched. File truncated.\n");
				j=8; i=strlen(msg)+8;			//break out of loops
			}
			
		}
	}
	if (fread(msg, 1, 1, stdin) != 0){
		printf("\nPPM file is not large enough to contain the whole message.\n");
		printf("Message truncated with first %lu characters remained.\n", msgCount);
	}
	while (!feof(fin)) {
		fwrite(&buffer, 1, 1, fout);
		fread(&buffer, 1, 1, fin);
	}
	
	free(msg);
	return 0;
}

/* MAIN for hide.c */
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
	printf("No errors in header.\n");
	
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
