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

int main (int argc, char *argv[]){
	// Check arguments
	if (argc > 2) {
		printf("More than 2 arguments. Take only the first two\n");
	}
	else if (argc < 2) {
		return awMan(1);
	}
	
	// Check file name
	int nameValidity = checkFileName(argv[1]);
	if (nameValidity != 0) {
		return nameValidity;
	}
	
	// Read file
	FILE *fin = fopen(argv[1], "rb");
	
	if (fin == NULL) {
		return awMan(4);
	}
	
	// check headers
	int width, height, color, notPass;
	char buffer;			//longest num posible is 65535 color space (TO BE USED LATER TOO!!!)
	notPass = headerChecking(fin, &width, &height, &color, &buffer);	//buffer is now first color, first pixel
	if(!notPass){
		printf("No errors in header\n");
	}
	else{
		return awMan(notPass);
	}

	// read message
	/*buffer until get 8bit => decrypt. Continue.*/
	printf("Encoded message: \n");
	int seeEnd = 0;
	while(!feof(fin) && !seeEnd){
		unsigned char letter = 0;		//00000000	//Only this which has to be unsigned
		int countBit=7;
		while(countBit >= 0 && !seeEnd){		//8 bits of msg in 8 bytes
			if (buffer%2 == 1 || buffer%2==-1){
				if (countBit == 7) {	
					seeEnd = 1;		
				}
				else {
					letter = letter + power(2, countBit);
				}
			}
			//printf("%d ", letter);
			fread(&buffer, 1, 1, fin);	
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
	printf("\n");
	
	
	return 0;
}
