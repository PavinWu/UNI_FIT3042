/*
 * A header file for asg1fn.c, which contains additional functions used to 
 * implement the program "hide" and "unhide" in FIT3042 assignments.
 * 
 * By: Pavin Wu (26916592)
 * Date: 14/4/17
 */

#include <stdio.h>	
#ifndef ASG1_H_	//guard?
#define ASG1_H_

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
int awMan(int awManCase);


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
int checkFileName(char fileName[]);


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
int checkArg (int argc, char *argv[], int argnum);


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
void skipWSorCom(char *ch, FILE *fp, int *skipped);


/* Function: isNumeric
 * -------------------
 * check if each ASCII character in str[] represents numeric 0-9
 * 
 * str[]: string with ASCII to be checked
 * 
 * return integer 1 if all numeric, 0 otherwise.
 */
int isNumeric(char str[]);


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
							int *height, int *color);
			
							
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
int headerChecking(FILE *fp, int *width, int *height, int *color, char *buffOut);


/* Function: power
 * ---------------
 * Calculate base^exponent (e.g. 2^3=8). Only works with integers.
 * 
 * base: base number
 * exp: exponent number
 * 
 * returns integer result.
 */
int power (int num, int power);


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
void toBinary (int num, int binary[], int arrSize);

#endif
