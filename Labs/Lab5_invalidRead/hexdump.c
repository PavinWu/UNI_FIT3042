#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char charToHex(int ch){
	
	if(ch>=10){
		//"A" is ASCII 65
		ch += 55;
	}
	else{
		//"0" is ASCII 48
		ch += 48;
	}
	return (char) ch;
}

int main (int argc, char *argv[]){
	FILE *hexf = fopen(argv[1], "rb");
	if (hexf == NULL){	//points to NULL
		return 1;
	}
	int buffer;	//one hex number is 4 bits. One char is 8 bit
	//char *hex = (char *)malloc(sizeof(char)*2);		///*???*/// why cast malloc?
	//fgetc vs getc??
	//why fgetc cast to int???
	//how to reference memory of buffer/16 ??
	
	while((buffer = fgetc(hexf)) != EOF){	
		printf("%c", charToHex(buffer/16));	
		printf("%c ", charToHex(buffer%16));	//least sig bit
	}
	printf("\n");
	
	fclose(hexf);
	
	return 0;
}
