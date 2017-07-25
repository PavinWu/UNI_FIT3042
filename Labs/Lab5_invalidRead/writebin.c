#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char *argv[]){
	FILE *hexf = fopen(argv[1], "wb");
	
	if (hexf == NULL){
		printf("No input file\n");
		return 1;
	}
	char someStr[] = "The quick brown fox jumps over the lazy dog";
	fwrite(someStr, sizeof(char)*strlen(someStr), 1, hexf);
	return 0;
}
