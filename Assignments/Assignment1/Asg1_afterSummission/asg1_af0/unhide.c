#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "asg1fn.h"

// output num^power. Only works with integers.
int power (int num, int power){
	int i;
	int result=1;
	for(i=0; i<power; i++){
		result*=num;
	}
	return result;
}

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
