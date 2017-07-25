#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, char * argv[]){
	char buff;
	int numChar[26];
	int i;
	for (i=0; i<26; i++){
		numChar[i] = 0;
	}
	
	FILE *fp;
	fp = fopen(argv[1], "r");
	if (fp==NULL){
		printf("No such file. Attempt to append .txt.\n");
		char *nameT = (char *) malloc(strlen(argv[1])+5);
		
		nameT = strcat(argv[1], ".txt");
		
		fp = fopen(nameT, "r");
		if (fp==NULL){
			printf("No such file\n");
			return 1;
		}
		free(nameT);
	} 
	
	while (!feof(fp)){
		fread(&buff, 1, 1, fp);
		if((buff>=65&&buff<=90) || (buff>=97&&buff<=122)){
			if(buff>=97){
				numChar[buff-97]++;
			}
			else if(buff>=75){
				numChar[buff-65]++;
			}
		}
	}
	
	for(i=0; i<26; i++) {
		printf("%c: %d\n", i+65, numChar[i]);
	}

	fclose(fp);
	return 0;
	}
