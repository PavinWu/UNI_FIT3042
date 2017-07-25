#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "asg1fn.h"

int main(int argc, char *argv[]) {
	// Check arguments
	if (argc > 3) {
		printf("More than 2 arguments. Take only the first two\n");
	}
	else if (argc < 3) {
		return awMan(1);
	}
	
	// Check file name
	int nameValidity = checkFileName(argv[1]);
	if (nameValidity != 0) {	//0 if valid
		return nameValidity;	
	}
	
	// Read file
	FILE *fin = fopen(argv[1], "rb");
	
	if (fin == NULL) {
		return awMan(4);
	}
	
	// check headers
	int width, height, color, notPass;
	char buffer;			//longest num posible is 65535 color space (THIS VAR also TO BE USED LATER!!)
	notPass = headerChecking(fin, &width, &height, &color, &buffer);	//buffer is now first color, first pixel
	if(!notPass){
		printf("No errors in header\n");
	}
	else{
		return awMan(notPass);
	}
	
	//Write header to output
	FILE *fout = fopen(argv[2], "wb");
	char headerBuff[7] = {'P', '6', '\n', '\0'};	//mainBuffer is size 6 (5+'\0') + one \n
	int len;
	
	int i;
	for(i=0; i<4; i++) {
		switch (i) {
			case 0:
				break;
			case 1:
				sprintf(headerBuff, "%i\n", width);
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
	}
	
	// Prep Take & Write message
	float maxChar = (float) (width*height)*3/8;		//max character image can take
	int maxChar_int = (int) maxChar - 1;			//round down by cast to int. Minus 1 to save for \0
	
	printf("Message must be no more than %d characters.\n", maxChar_int);
	printf("Input message of no more than 500 characters at a time.\n");
	
	int j;	//counter variable
	int continueMsg = 1;	//0 is not continue inputting message
	int totalMsgLoop = 0;	//total number of loop of message so far. Used to keep track of total number of characters 
	char bufferMsg[501];
	int binary[8] = {0,0,0,0,0,0,0,0};
	int numCharLoop;	//possible no. of char in a loop
	
	// Take & Write message
	while (continueMsg && (totalMsgLoop*500) < maxChar_int) {
		totalMsgLoop++;

		numCharLoop = maxChar_int-totalMsgLoop*500;	
		
		if (numCharLoop < 500) {		//last loop might allow only char less than 500 (for small img)
			printf("Now can take only %d characters more.\n", numCharLoop);
			printf("Message: \n");
			fgets(bufferMsg, numCharLoop, stdin);
			if (strlen(bufferMsg) >= numCharLoop) {
				printf("\nExceed characters limit\n");
			}
			numCharLoop = 0;
		}
		else {
			printf("Message: \n");
			fgets(bufferMsg, 500, stdin);
		}
		
		
		if (numCharLoop > 0){
			//create new ppm with message encoded
			for (i=0; i<strlen(bufferMsg); i++){
				//ASCII to binary
				toBinary(bufferMsg[i], binary, 8);	//8 bit binary of one character message
				
				//gotta get new byte from image 8 times (1 bit of message per each image's byte)
				//if ASCII is even: least sig digit already 0 => change only if message bit is 1. 
				//Other way when ASCII is odd
				for (j=0; j<8; j++){
					if (buffer%2==0 && binary[j]==1) {
						buffer+=1;
					}
					else if ((buffer%2==1||buffer%2==-1) && binary[j]==0){
						buffer-=1;
					}
					fwrite(&buffer, 1, 1, fout);
					fread(&buffer, 1, 1, fin);	//Read ppm (edit before read: have left over byte from before loop)
				}
			}		
		}
		
		// repeat
		int option;
		if (numCharLoop > 0){
			printf("\nDo you wish to continue?(Y/N): ");
			option = getchar();
		}
		else {
			printf("Exceed characters limit\n");
		}
		if (numCharLoop <= 0 || option==78 || option==110) {	//if N or n
			for (j=0; j<8; j++){			//write '\0'
				if (buffer%2==1 || buffer%2==-1) {
					buffer-=1;		
				}
				fwrite(&buffer, 1, 1, fout);
				fread(&buffer, 1, 1, fin);
			}
			continueMsg = 0;
		}
		else {
			printf("\n");
		}
		while ((option = getchar()) != '\n' && option != EOF) { }	//flush stdin
	}
	//will not encounter feof up there (because of condition of while)
	//might encounter it down here
	while(!feof(fin)){
		fwrite(&buffer, 1, 1, fout);
		fread(&buffer, 1, 1, fin);
	}

	fclose(fin);
	fclose(fout);

	return 0;
}
