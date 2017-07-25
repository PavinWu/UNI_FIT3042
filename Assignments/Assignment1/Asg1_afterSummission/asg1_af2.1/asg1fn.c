/*
 * A c source file containing additional functions used to implement
 * the program "hide" and "unhide" in FIT3042 assignments.
 * 
 * By: Pavin Wu (26916592)
 * Date: 15/4/17
 */

#include "asg1fn.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* 
 * Function: awMan
 * ---------------
 * Output error to stdout based on error code received from other functions
 *  
 * awManCase: an integer >0, passed on from other functions to indicate 
 * 		different types of errors
 * 
 * return awManCase: return the same number out, in case it is to be
 * 		used later for other purposes
 */
int awMan(int awManCase){
	switch(awManCase){
		case 1:
			printf("Input error\n");
			break;
		case 2:
			printf("Invalid file Name\n");
			break;
		case 3:
			printf("Cannot allocate memory for message\n");
			break;
		case 4:
			printf("File open error\n");
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
			printf("Color should be numeric of value between 1 and 65536\n");
			break;
		case 9:
			printf("Header incomplete\n");
			break;
		default:
			//printf("");
			break;
	}
	return awManCase;
} 

/* 
 * Function: checkFileName
 * ---------------
 * check if fileName[] is of a valid ppm type
 *  
 * fileName[]: string of file name to be checked
 * 
 * return: an integer >= 0 indicating errors occured;
 * 		0 if no error, 
 *  	2 if invalid file name.
 */
int checkFileName(char fileName[]){
	int i = 0; 
	int valid = 0;
	while (fileName[i]!='\0'){
		if ((fileName[i]=='.' && valid==0) ||
			(fileName[i]=='p' && (valid==1 || valid==2)) ||
			(fileName[i]=='m' && valid==3)) valid++;
		else valid = 0; 
		i++;
	}
	if (valid != 4) { 	//'m' is not last character in file name
		return 2;
	}
	return 0;	
}

/* Function: checkArg
 * ------------------
 * Check if argument is valid
 * i.e. suitable number of arguments and 2nd argument has a valid ppm file type 
 * 
 * argc: integer number of argument received from stdin
 * argv: argument arrays with file name to be checked pointed to by argv[1]
 * argnum: integer for number of argument we actually want
 * 
 * return integer error code:
 * 		0 for no error,
 * 		1 for argument input error,
 * 		2 for file name error.
 * */
int checkArg (int argc, char *argv[], int argnum){
	// Check number of arguments
	if (argc > argnum) {
		printf("More than %i arguments. Take only the first two\n", argnum);
	}
	else if (argc < argnum) {
		printf("Need %i arguments. ", argnum);
		return 1;
	}
	
	// Check file name
	if (checkFileName(argv[1])){	// 0 when valid, 2 if invalid
		return 2;
	}
	
	return 0;
}

/* 
 * Function: skipWorCom
 * ---------------
 * skip a section of whitespace or/and comments in header of ppm file.
 * Runs until meeting a non-whitespace character (or end of line in case 
 * of comment section) 
 *  
 * ch: pointer to character in header to be read.
 * 		At end of function, Will point to the character just after 
 * 		whitespace or/and comment section
 * fp: pointer to ppm file (which has character *ch points to)
 * 		Will also be updated as function goes through whitespace/comments
 * *skipped: integer to indicate if there is any skipping. 
 * 		1 if yes, 0 otherwise.
 * 
 * No return value (output through pointers)
 */
void skipWSorCom(char *ch, FILE *fp, int *skipped) {
	int comment = 0;
	*skipped = 0;
	while (*ch==' ' || *ch=='\t' || *ch=='\r' || *ch=='\n' || *ch=='#' || comment) {
		*skipped = 1;
		if (*ch=='#' && !comment) comment = 1;	//start comment section
		if (comment && *ch=='\n') comment = 0;	//stop comment section
		fread(ch, 1, 1, fp);
	}
}

/* Function: isNumeric
 * -------------------
 * check if each ASCII character in str[] represents numeric 0-9
 * 
 * str[]: string with ASCII to be checked
 * 
 * return integer 1 if all numeric, 0 otherwise.
 */
int isNumeric(char str[]){
	int i;
	for (i=0; i<strlen(str); i++){
		if(str[i]<'0' || str[i]>'9'){
			return 0;
		}
	}
	return 1;
}

/* Function: headerContentChecking
 * -------------------------------
 * check validity of specific content in ppm header
 *  
 * headerChecked: pointer to headerChecked array, to keep track of checking progress
 * mainBuff: pointer to char array, containing the header content to be checked
 * 
 * width: pointer integer width value contained in header
 * height: pointer integer height value contained in header
 * color: pointer integer color depth value contained in header
 * The three parameters above will change its value only when the corresponding
 * 		content in header has been checked.
 * 
 * return integer error code:
 * 		0: no error
 * 		5: magic number not P6
 * 		6: width not a number
 * 		7: height not a number
 * 		8: color not a number, or color depth is greater than 16bits
 */
int headerContentChecking (int *headerChecked, char *mainBuff, int *width, 
							int *height, int *color){
	
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
		*width = atoi(mainBuff);	//convert ASCII width to integer
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
		if(*color >= 65536 && *color <= 0) {
			return 8;
		}
		headerChecked[3] = 1;
	}
	return 0;
}

/* Function: headerChecking
 * ------------------------
 * check that ppm's header is valid.
 * 
 * fp: pointer to ppm file to check
 * width: pointer to width integer value of ppm
 * height: pointer to height integer value of ppm
 * color: pointer to color depth integer value of ppm
 * buffOut: pointer to character (1st byte of binary) just after the header section
 * 
 * returns an integer indicating errors encountered 
 */
int headerChecking(FILE *fp, int *width, int *height, int *color, char *buffOut){
	int headerChecked[4] = {0,0,0,0};
	
	char sgBuff;		//singleBuffer (buffer of length one)
	char mainBuff[6];	//main buffer of size 6 (to put content of header in)
	char temp;	
	
	int buffInd = 0;	//index in mainBuff to put character in
	while (!feof(fp)) {
		if (!headerChecked[3]) {		//if header hasn't been entirely checked
			int skipped;
			fread(&sgBuff, 1, 1, fp);
			skipWSorCom(&sgBuff, fp, &skipped);
			mainBuff[buffInd] = sgBuff;			
			
			if (skipped) {		//if skipped a whitespace or comment section
				/* After skipping, sgBuff will have extra value of next content 
				 * (not a part of current one we want) => hold off in temp */
				temp = sgBuff;			
				/* put '\0' so it can be dealt with like a normal string */
				mainBuff[buffInd] = '\0';
				
				int contentErr = headerContentChecking(headerChecked, mainBuff, 
								width, height, color);
				if(contentErr){ 
					return contentErr;
				}
				skipped = 0; 
				buffInd = 0; 
				mainBuff[0] = temp;
			}
			buffInd++;
		}
		else {
			*buffOut = mainBuff[0];
			return 0;
		}
	}
	return 9;
	/* Will never get to this point except the case when file terminates
	 * prematurely, implying header is incomplete*/
}

/* Function: power
 * ---------------
 * Calculate base^exponent (e.g. 2^3=8). Only works with integers.
 * 
 * base: base number
 * exp: exponent number
 * 
 * returns integer result.
 */
int power (int base, int exp){
	int result=1;
	int i;
	for(i=0; i<exp; i++){
		result*=base;
	}
	return result;
}

/* Function: toBinary
 * ------------------
 * convert a decimal number to binary number
 * 
 * num: integer storing decimal number to be converted
 * binary: integer array to store converted binary
 * arrSize: size of binary array
 * 
 * WARNING: Assumes binary calculated from num fits into the size of binary[].
 * 		Will return segfault if it does not fit (to be implmented later?) 
 */
void toBinary (int num, int binary[], int arrSize){
	int i= arrSize - 1;		//Fill in binary digit from end of array
	while(num > 0){
		binary[i] = num%2;
		num/=2;
		i--;
	}
	while(i>0){
		binary[i] = 0;		//if there are unfilled digit in binary array
		i--;
	}
} 
