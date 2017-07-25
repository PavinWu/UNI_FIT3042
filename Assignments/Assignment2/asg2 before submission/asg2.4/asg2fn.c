/*
 * A c source file containing additional functions used to implement
 * the program "hide" and "unhide" specifically for FIT3042 assignment2.
 * For functions which are shared across two assigments, see asg1fn.c
 * 
 * By: Pavin Wu (26916592)
 * Date: 16/4/17
 */
 
#include "asg1fn.h"
#include "asg2fn.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/* Function: checkFlag
 * -------------------
 * check for flag defined in assignment 2 and return associated integer
 * 
 * return 1 for -m flag (Task1) 
 * 		  2 for -p flag (Task2)
 * 		  3 for -s flag (Task3)
 * 		 -1 if not used a single m,p,s flag
 * 		  0 for same implementation as assignment 1
 */
int checkFlag (char *flag){
	if (!strcmp(flag, "-m")) {	//0 if matches
		return 1;
	}
	else if (!strcmp(flag, "-p")) {
		return 2;
	}
	else if (!strcmp(flag, "-s")) {
		return 3;
	}
	else if (!strncmp(flag, "-", 1)){
		return -1;
	}
	return 0;
}


/* Function: checkArg2
 * -------------------
 * check if the arguments (apart from flag) for assignment2 programs are valid.
 * 
 * 
 */
 /* Meaning of different flags
  * flag==1: -m: multiple ppm at a time
  * flag==2: -p: parallel ppm processing
  * flag==3: -s: show a single ppm
  * flag==0: like assignment 1
  * 
  * NOTE: Don't need to check input file name with flag==1 or 2 since it is input
  *  	  as ordinary text. Will check again when open the files.
  * */
int checkArg2 (int flag, char *argv[]){
	
	if (flag==1){
		// check if number-of-files > 0 
		if (atoi(argv[2])<1 || atoi(argv[2])>255){
			/* can input number, following by text (since use atoi)
			 * if input non-numeric => no number to be converted
			 * 	=> gives 0
			 */
			return 12;
		}
	}
	else if (flag==0 || flag==3){
		// check file name ends with .ppm
		if (checkFileName(argv[2])){	// 0 when valid, 2 if invalid
			return 2;
		}
	}
	
	return 0;
}

/* Function: changeNameNum
 * ---------------------
 * change file name numbering to i
 * 
 * i is between 0 to 254 (can have between 1 to 255 ppm pictures)
 */
void changeNameNum(char *name, int i){
	int len = strlen(name);	//strlen == index of '\0'
	char num[4];
	
	if (i > 99){ 	//3 digits
		sprintf(num, "%i", i);
	}
	else if (i > 9){	//2 digits
		sprintf(num, "0%i", i);
	}	
	else {		//1 digit
		sprintf(num, "00%i", i);
	}
	
	*(name+len-5) = num[2];		//{..., '0','0','0','.','p','p','m','\0'}
	*(name+len-6) = num[1];
	*(name+len-7) = num[0];
}
