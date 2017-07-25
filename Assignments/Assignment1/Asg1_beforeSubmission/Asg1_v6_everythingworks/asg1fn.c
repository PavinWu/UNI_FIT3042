#include "asg1fn.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int awMan(int awManCase){
	switch(awManCase){
		case 1:
			printf("Input error\n");
			break;
		case 2:
			printf("Invalid file Name\n");
			break;
		case 3:
			printf("File type not ppm\n");
			break;
		case 4:
			printf("No such file or directory\n");
			break;
		case 5:
			printf("PPM magic number not P6\n");
			break;
		case 6:
			printf("Width not numbers\n");
			break;
		case 7:
			printf("Height not numbers\n");
			break;
		case 8:
			printf("Color should be numeric of value < 65536\n");
			break;
		default:
			//printf("");
			break;
	}
	return awManCase;
} 

int checkFileName(char fileName[]){
	int i = 0;
	while (fileName[i]!=46 && fileName[i]!=0){
		//not dot and not zero
		i++;
	}
	if (fileName[i] == 0) {
		return awMan(2);
	}
	else if (fileName[i] == 46) {
		char fileType[4];
		int fileType_len = strlen(fileName)-i-1;	//to take away name, dot
		if (fileType_len != 3) {
			return awMan(3);
		}
		
		strncpy(fileType, fileName+i+1, fileType_len);
		fileType[3] = '\0';
		if (strcmp(fileType, "ppm\0")) {
			// if not equal, will be non-zero
			return awMan(3);
		}
	}
	return 0;
}

void skipWSorCom(char *ch, FILE *fp, int *skipped) {
	// skip White Space or Comment
	// return ch as: first non white space character after WS or comment
	int seeComment = 0;
	*skipped = 0;
	while (*ch==32 || *ch==9 || *ch==13 || *ch==10 || seeComment) {
		*skipped = 1;
		if (*ch==35 && !seeComment) {seeComment = 1;}
		if (seeComment && *ch==10) {seeComment = 0;}
		fread(ch, 1, 1, fp);
	}
}

int isNumeric(char str[]){
	int i;
	for (i=0; i<strlen(str); i++){
		if(str[i]<48 || str[i]>57){
			return 0;
		}
	}
	return 1;
}


int headerChecking(FILE *fp, int *width, int *height, int *color, char *buffOut){
	int headerChecked[4] = {0,0,0,0};
	
	char sgBuff;	//(singleBuffer := buffer of length one)
	char mainBuff[6];
	char temp;	
	
	int buffPos = 0;
	while (!feof(fp)) {
		if (!headerChecked[3]) {	//if header not checked
			int skipped;
			fread(&sgBuff, 1, 1, fp);
			skipWSorCom(&sgBuff, fp, &skipped);	//skip white space or comment
			mainBuff[buffPos] = sgBuff;
						
			if (skipped) {
				temp = sgBuff;
				mainBuff[buffPos] = '\0';
				
				if(!headerChecked[0]){			//check magic number

					if (strcmp(mainBuff, "P6")){
						return 5;
					}
					headerChecked[0] = 1;
				}
				else if (!headerChecked[1]) {	//check width
					if(!isNumeric(mainBuff)) {   
						return 6;
					}
					*width = atoi(mainBuff);
					headerChecked[1] = 1;
				}
				else if(!headerChecked[2]) {    //check height
					if(!isNumeric(mainBuff)) {
						return 7;
					}
					*height = atoi(mainBuff);
					headerChecked[2] = 1;
				}
				else if(!headerChecked[3]) {    //check color
					if(!isNumeric(mainBuff)) {
						return 8;
					}
					*color = atoi(mainBuff);
					if(*color >= 65536) {
						return 8;
					}
					headerChecked[3] = 1;
				}
				
				skipped = 0;
				buffPos = 0;
				mainBuff[0] = temp;
			}
			buffPos++;	//watch this
		}
		else {
			*buffOut = mainBuff[0];
			return 0;
		}
	}
}

void toBinary (int num, int binary[], int arrSize){
	//Assumes binary fo num fits the size of binary array 
	int i= arrSize - 1;
	while(num > 0){
		binary[i] = num%2;
		num/=2;
		i--;
	}
	while(i>0){
		binary[i] = 0;	//reset
		i--;
	}
} 
