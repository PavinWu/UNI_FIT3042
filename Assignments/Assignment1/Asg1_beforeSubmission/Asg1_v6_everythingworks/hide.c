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
	unsigned char buffer;			//longest num posible is 65535 color space (TO BE USED LATER TOO!!!)
	notPass = headerChecking(fin, &width, &height, &color, &buffer);	//buffer is now first color, first pixel
	if(!notPass){
		printf("No errors in header\n");
		printf("%d, %d, %d\n", width, height, color);
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
	
	// Prep Take message
	float maxChar = (float) (width*height)*3/8;
	int maxChar_int = (int) maxChar - 1;	//round down. -1: save for \0
	printf("Message must be no more than %d characters.\n", maxChar_int);
	printf("Input message of no more than 500 characters at a time.\n");
	
	int j;
	int continueMsg = 1;
	int totalMsgLoop = 0;
	unsigned char bufferMsg[501];
	int binary[8] = {0,0,0,0,0,0,0,0};
	
	// Take message
	while (continueMsg && totalMsgLoop*500 < maxChar_int) {
		printf("Message: ");
		totalMsgLoop++;
		
		int charLastLoop = maxChar_int-totalMsgLoop*500-1;	//no. of char in last loop
		if (charLastLoop < 500) {		//last loop might allow only char less than 500
			fgets(bufferMsg, charLastLoop, stdin);
		}
		else if (charLastLoop < 0) {
			printf("Exceed characters limit");
		}
		else {
			fgets(bufferMsg, 500, stdin);
		}
		
		//convert message to binary. 
		for (i=0; i<strlen(bufferMsg); i++){
			//ASCII to binary
			if (bufferMsg[i] == '\0') {
				continue;
			}
			toBinary(bufferMsg[i], binary, 8);	//got binary of one character here => gotta get new byte from image 8 times

			//edit it
			//if ASCII even: least sig dig is 0 => change only if message digit is 1
			//	vice versa for ASCII odd
			for (j=0; j<8; j++){
				if (buffer%2==0 && binary[j]==1) {
					buffer+=1;
				}
				else if ((buffer%2==1||buffer%2==-1) && binary[j]==0){
					buffer-=1;
				}
	
				fwrite(&buffer, 1, 1, fout);
				fread(&buffer, 1, 1, fin);	//Read ppm (edit before read: have left over byte)
			}
			//get -1 for 255 ascii !??
		}		
		
		// repeat
		printf("\nDo you wish to continue?(Y/N): ");
		int option = getchar();
		if (option==78 || option==110) {
			buffer = '\0';
			fwrite(&buffer, 1, 1, fout);
			fread(&buffer, 1, 1, fin);
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
