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
 * Date: 15/4/17
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "asg1fn.h"
#include "asg2fn.h"


/* Function: readMsgfromPPM
 * -----------------
 * read secret message hidden in *fin P6 ppm based on steganography technique.
 * 
 * buffer: first byte of ppm's binary data. Received after successful headerChecking
 * fin: pointer to the P6 ppm file
 * 
 * no return value. Potential file open error is checked in main.
 * 
 * */
void readMsgfromPPM (char buffer, FILE *fin) {	
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

int readMsgfromPPMs1 (char buffer, FILE *fin, int *countBit, char *letter) {	
	while(!(feof(fin))){
		while((*countBit)>=0 && !feof(fin)){			//8 bits of msg in 8 bytes
			if (buffer%2==1 || buffer%2==-1){
				if ((*countBit) == 7) {	
					/* ASCII message should be only 8bit 
					 * But 2^7=128 => beyond 8bit range => error in message*/
					return 13;	
				}
				else {
					*letter = (*letter) + power(2, (*countBit));
				}
			}
			(*countBit)--;
			fread(&buffer, 1, 1, fin);
		}
		if (feof(fin)){
			return 4;
		}
		if(letter=='\0' && (*countBit)<0){
			fclose(fin);
			return 0;
		}
		else {
			printf("%c", *letter);	
			*countBit = 7;
			*letter = 0;
		}
		
	}
	return 0;
}

/* MAIN for unhide.c */
// should be able to share code between different modes.
// *But Later*
int main (int argc, char *argv[]){
	int err; int i=0;
	int opmode = checkFlag(argv[1]);
	int width, height, color;
	char buffer;
	
	// Check arguments
	if (opmode == 0){
		err = checkArg(argc, argv, 2);
		if(err) {return awMan(err);}
		
		// Read file
		FILE *fin = fopen(argv[1], "rb");
		if (fin == NULL) {return awMan(4);}
		
		// check headers
		err = headerChecking(fin, &width, &height, &color, &buffer);	
			//buffer now has value of first color of first pixel
		if(err) {return awMan(err);}
		printf("No errors in header\n");

		// read message
		/*buffer until get 8bit => decrypt. Continue.*/
		printf("Encoded message: \n");
		readMsgfromPPM(buffer, fin);
		printf("\n");
	}
	else if (opmode == 1){
		// don't need to check arg if unhide (argv[2] can be anything non-empty)
		printf("Encoded message: \n");
		
		char *finname = (char *) malloc(sizeof(char)*(strlen(argv[2])+9));
		char postfix[9] = {'-','0','0','0','.','p','p','m','\0'};
		finname = strdup(argv[2]);
		strcat(finname, postfix);
	
		int *countBit = (int *) malloc(sizeof(int));  
		*countBit = 7;	//most significant bit comes first
		char *letter = (char *) malloc(sizeof(char)); 
		*letter = 0;	//i.e. 00000000	
		FILE *fin = fopen(finname, "rb");
		
		while(fin!=NULL) {		//while still files left or not seeEnd
			//check headers
			err = headerChecking(fin, &width, &height, &color, &buffer);	
			if(err) {return awMan(err);}
			
			//read message from ppm
			err = readMsgfromPPMs1(buffer, fin, countBit, letter);
			if (err==13 || err==0) {return err;}
			
			fclose(fin);
			i++;
			changeNameNum(finname, i); 	// increment file name
			fin = fopen(finname, "rb");
		}
		printf("\n");
		if(fin == NULL){
			// loop finished vs file error?
			
		}
		
		free(finname);
		free(countBit);
		free(letter);
	}
	return 0;
}
