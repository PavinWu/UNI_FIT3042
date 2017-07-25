#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void reverse(char * string){
	
	int len = strlen(string);		//string[index] doesn't work
	int i;
	/*
	for (i=0; i<len; i++){
		printf("%c %c\n", *(string+i), *(string+len-i-1));
	}*/
	char temp; 

	for (i=0; i<len/2; i++){
		temp = *(string+i);
		*(string+i) = *(string+len-i-1);
		*(string+len-i-1) = temp;
	}
	printf("\n");
	
	return;
}

int main (int argc, char *argv[]) {
	
	//char *inStr = *argv[1] 	//initialization makes pointer from integer without a case???
	
	int len = strlen(argv[1]);	//have to assign return value of strlen directly (rather than putting to function!)
	
	/*char *inStr = (char *) malloc(sizeof(char)*len);	//strlen takes (char *) if string not defined from array notation
	inStr = argv[1];
	reverse(inStr);	
	*/	//This gives munmap_chunk(): invalid pointer error ????
	reverse(argv[1]);
	
	int i;
	for (i=0; i<len; i++){
		printf("%c", *(argv[1]+i));
	}
	printf("\n");
	
	//free(inStr);
	return 0;
}
