#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char *argv[]){
	
	/*
	char *x[] = {"huh", "Noooooooooooo"};
	
	printf("%s\n", x[1]);	\\%s expects a pointer
	*/
	
	//printf("%i\n", atoi("0143asd2	2as"));
	
	/*
	char name1[7] = {'t','e','s','t','m','s','g'};
	char name[8] = {'t','e','s','t','m','s','g','\0'};
	FILE *fin = fopen(name1, "r");
	//FILE *fin = fopen(name, "r");
	
	//BOTH OPEN SUCCESSFULLY!	
	
	if (fin==NULL) printf("NULL");
	else { 
		char buffer;
		fread(&buffer, 1, 1, fin);
		printf("%c\n", buffer);
	}
	* fclose(fin);
	* */
	
	/*
	if (strcmp(argv[1], "-m")) {
		return 1;
	}*
	*/
	
	/*
	int i=0;
	for (i; i<10; i++){
		printf("%d ", i);
	}
	printf("\n");
	*/
	
	/*
	printf("%s", argv[1]);
	printf("%d\n", strcmp(argv[1], ""));
	*/
	
	/*char *s;
	s = strdup("yeah mate");
	printf("%s\n", s);*
	* 
	*/
	
	if (fopen("./testmsg/testmsg2 (copy 3)","r") == NULL){
		printf("Noooooo\n");
	}
	return 0;
}
