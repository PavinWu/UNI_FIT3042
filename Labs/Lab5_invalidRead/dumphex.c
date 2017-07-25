#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFSIZE 1000

// convert characters representing two bits of hex number to actual hex number
// return *a number* with char type
char charHexToChar(char ch){
	char hex = 0;
	if(ch >= 65 && ch <= 70){		//if A-F (10-15) (ASCII 65-70)
		hex = ch - 55;
	}
	else if (ch >= 48 && ch <= 57){	//if 0-10
		hex = ch - 48;
	}
	return hex;
}

int main (int argc, char *argv[]){
	//just write char using b flag. No need to convert explicitly?
	FILE *fout = fopen(argv[1], "wb");
	if (fout == NULL){
		printf("No file name specified\n");
		return 1;
	}
	
	char buffer[BUFFSIZE];
	
	fgets(buffer, BUFFSIZE, stdin);
	
	int i; unsigned char buffadd = 0; int count = 0;
	for(i = 0; buffer[i]!='\0'; i++){
		if(buffer[i]!=' ' && buffer[i]!='\n'){
			//if((buffer[i]<65 && buffer[i]>70) || (buffer[i]<48 && buffer[i]>57)){
			if((buffer[i]<'0') || (buffer[i]>'9' && buffer[i]<'A') || (buffer[i]>'F')){
				printf("Input invalid hex number\n");
				return 1;
			}
				
			if(count == 1){		//write 8 bit at a time
				buffadd += (unsigned char) charHexToChar(buffer[i]);
				fputc(buffadd, fout);
				buffadd = 0;
			}
			else if (count == 0){
				buffadd += (unsigned char) charHexToChar(buffer[i])*16;
			}
			else{
				printf("Non 2-bit hexidecimal number detected\n");
				return 1;
			}
			count++;
		}
		else{
			count = 0;
		}
	}
	
	fclose(fout);
	return 0;
}

