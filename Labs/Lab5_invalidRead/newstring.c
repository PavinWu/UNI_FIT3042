#include <stdio.h>
#include <stdlib.h>
#include "newstring.h"
#include <string.h>


int newstrlen(Newstring *str){
	int length;	//what if we put lenght = 0 here instead of for loop?
	char *temp = str -> string;		//str -> *string doesn't work
	for(length = 0; *(temp+length) != '\0'; length++);
	return length;	//will be +1 for \n if received by fgets
}

Newstring * newstrcat(Newstring *str1, Newstring *str2){
	Newstring *bigstr = (Newstring *) malloc(sizeof(Newstring));
	bigstr -> len = str1->len + str2->len;
	bigstr -> string = (char *) malloc(sizeof(char)*(bigstr->len));
	
	int i; int j;
	for(i=0; *((str1->string)+i) != '\0'; i++){		//not include null character
		*((bigstr -> string) + i) = *((str1->string+i));
	}

	for(j=0; j<=(str2->len); j++){
		*((bigstr -> string) + j + i) = *((str2->string+j)); 
	}
	
	return bigstr;
}//put to new string rather than append at str1 => no buffer overrun like original strcat?

Newstring * newstrcpy(Newstring *strdest, Newstring *strsrc){
	strdest = (Newstring *) malloc(sizeof(strsrc));
	strdest -> len = strsrc -> len;
	strdest -> string = (char *) malloc(sizeof(char)*(strdest -> len));
	
	int i;
	for(i=0; i<=(strdest->len); i++){
		*((strdest->string)+i) = *((strsrc->string)+i);
	}
	
	return strdest;
}

int main (int argc, char *argv[]){
	Newstring *str1 = (Newstring *)malloc(sizeof(Newstring));	//why cast??
	str1 -> string = (char *)malloc(sizeof(char)*100);			//string of len 100
	// can use strdup too (will allocate memory for you)
	//		use some buffer to gets, duplicate the string pointer to our pointer  
	// malloc returns a pointer, no need to dereference
	fgets((str1 -> string), 100, stdin);
	str1 -> len = newstrlen(str1);
	//printf("%s\n", (str1 -> string));
	
	Newstring *str2 = (Newstring *)malloc(sizeof(Newstring));	//why cast??
	str2 -> string = (char *)malloc(sizeof(char)*100);
	fgets((str2 -> string), 100, stdin);
	str2 -> len = newstrlen(str2);
	
	//will be pointed to, no need to allocate
	Newstring *bigstr; //= (Newstring *)malloc(sizeof(Newstring)*2);
	bigstr = newstrcat(str1, str2);
	printf("newstrcat result: %s\n", (bigstr -> string));
	
	Newstring *somestr;
	somestr = newstrcpy(somestr, str1);
	printf("newstrcpy result: %s\n", (somestr -> string));
	
	free(str1->string); free(str2->string);
	free(bigstr->string); free(somestr->string);
	
	free(str1); free(str2); 
	free(bigstr);	//identical pointer allocated somewhere else. Can free. 
	//(can't free direcly if not identical)
	free(somestr);
	
	return 0;
}
