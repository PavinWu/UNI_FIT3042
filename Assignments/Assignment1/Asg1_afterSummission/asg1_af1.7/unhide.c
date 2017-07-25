/* 
 * unhide.c: 
 * unhiding messages stored in a binary ppm file (with magic number P6).
 * 		The ppm must be encoded from a program compiled from hide.c, which hid
 * 		the messae using steganography technique. Other implementations of same
 * 		steganography may work, but with no warranty.
 * 		 
 * This file contains both main function and the functions considered major
 * part of the program. Other helper functions are contained in asg1fn.c.
 * 
 * This is a part of FIT3042 assignment1
 * 
 * By: Pavin Wu (26916592)
 * Date: 14/4/17
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "asg1fn.h"

void readMsg (char buffer, FILE *fin) {
	int countBit, seeEnd = 0;
	char letter;
	while(!seeEnd){
		letter = 0;			//i.e. 00000000	
		countBit = 7;		//most significant bit comes first
		while(countBit>=0 && !seeEnd){			//8 bits of msg in 8 bytes
			if (buffer%2==1 || buffer%2==-1){
				if (countBit == 7) {	
					/* ASCII message should be only 8bit 
					 * But 2^7=128 => beyond 8bit range => error in message*/
					printf("\n[non-ASCII character detected]\n");
					seeEnd = 1;		
				}
				else {
					letter = letter + power(2, countBit);
				}
			}
			if (feof(fin)){
				seeEnd = 1;
			}
			else {
				fread(&buffer, 1, 1, fin);
			}
			countBit--;
		}
		if(letter=='\0'){
			fclose(fin);
			seeEnd = 1;
		}
		else {
			printf("%c", letter);
		}
	}
	
}

int main (int argc, char *argv[]){
	// Check arguments
	checkArg(argc, argv, 2);
	
	// Read file
	FILE *fin = fopen(argv[1], "rb");
	if (fin == NULL) {return awMan(4);}
	
	// check headers
	int width, height, color;
	char buffer;
	int err = headerChecking(fin, &width, &height, &color, &buffer);	
		//buffer now has value of first color of first pixel
	if(err) {return awMan(err);}
	printf("No errors in header\n");

	// read message
	/*buffer until get 8bit => decrypt. Continue.*/
	printf("Encoded message: \n");
	readMsg(buffer, fin);
	printf("\n");
	
	return 0;
}
