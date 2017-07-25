#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "asg1fn.h"

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
	unsigned char buffer;			//longest num posible is 65535 color space (TO BE USED LATER TOO!!!)
	notPass = headerChecking(fin, &width, &height, &color, &buffer);	//buffer is now first color, first pixel
	if(!notPass){
		printf("No errors in header\n");
		printf("%d, %d, %d\n", width, height, color);
	}
	else{
		return awMan(notPass);
	}
	
	printf("%d, %d\n", buffer, power(2,3));

	// read message
	/*buffer until get 8bit => decrypt. Continue.*/
	printf("Encoded message: ");
	int seeEnd = 0;
	while(!feof(fin) && !seeEnd){
		unsigned char letter = 0;		//00000000
		int countBit=7;
		while(countBit >= 0 && buffer!='\0'){		//8 bits of msg in 8 bytes
			if (buffer%2 == 1){
				letter = letter + power(2, countBit);
			}
			//printf("%d ", letter);
			fread(&buffer, 1, 1, fin);	
			countBit--;
		}
		printf("%c", letter);
		if(buffer=='\0'){
			seeEnd = 1;
		}
		
	}
	
	return 0;
}
