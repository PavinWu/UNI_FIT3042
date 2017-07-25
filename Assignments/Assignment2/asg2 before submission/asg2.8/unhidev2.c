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
		if(*letter=='\0' && (*countBit)<0){
			return -1;
		}
		if (feof(fin)){		//if may continue with another file
			return 4;
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
	int err=0; int i=0; char seeEnd=0;
	int width, height, color; char buffer;
	char *finname = NULL;
	int *countBit = (int *) malloc(sizeof(int)); 
	char *letter = (char *) malloc(sizeof(char));
	*countBit = 7;	//most significant bit comes first
	*letter = 0;	//i.e. 00000000	
	
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
		
	int opmodef=opmode+10;	//possibly different opmode to be used as flag to check arguments	
	err = checkArg2(opmodef, argv);
	if(err) {goto CLEARALL;}
	
	// Check arguments
	if (opmode == 0){
		finname = (char *) malloc(sizeof(char)*(strlen(argv[1])));
		strcpy(finname, argv[1]);
	}
	else if (opmode == 1){
		// don't need to check arg if unhide (argv[2] can be anything non-empty)
		finname = (char *) malloc(sizeof(char)*(strlen(argv[2])+9));
		char postfix[9] = {'-','0','0','0','.','p','p','m','\0'};
		strcpy(finname, argv[2]);
		strcat(finname, postfix);
	}
	
	FILE *fin = fopen(finname, "rb");
	if (fin == NULL) {
		err = 4;
		goto CLEARALL;
	}
	printf("Encoded message: \n");	
	while(fin!=NULL && !seeEnd) {		//while still files left or not seeEnd

			err = headerChecking(fin, &width, &height, &color, &buffer);	
			if(err) {goto CLEARALL;}
			err = readMsgfromPPMs1(buffer, fin, countBit, letter);
			if (err==13 || err==0) {goto CLEARALL;}
			else if (err==-1) {seeEnd=1;}
			
			fclose(fin); fin = NULL;	//not necessarily NULL after close(?)
			if (opmode == 1) {
				i++;
				changeNameNum(finname, i); 	// increment file name
				fin = fopen(finname, "rb");
			}
	}
	printf("\n");
	
	CLEARALL:
		free(countBit);
		free(letter);
		free(finname);
		if (err!=0 && err!=-1) {return awMan(err);}
		
	return 0;
}
