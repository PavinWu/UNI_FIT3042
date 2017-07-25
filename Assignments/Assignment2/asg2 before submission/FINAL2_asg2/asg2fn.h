/*
 * A header file for asg2fn.c, which contains additional functions used to 
 * implement the program "hide" and "unhide" in FIT3042 assignments.
 * 
 * By: Pavin Wu (26916592)
 * Date: 14/4/17
 */

#include <stdio.h>	
#ifndef ASG2_H_	//guard?
#define ASG2_H_

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
int checkFlag (char *flag);


/* Function: checkArg2
 * -------------------
 * check if the arguments (apart from flag) for assignment2 programs are valid, 
 * based on the flag.
 * 
 * return 0: no errors, 
 * 		  1: some arguments missing
 * 		  2: invalid file names
 * 		  12: number of ppm to use with -m flag is <1 or >255
 */
 /* Meaning of different flags
  * flag==1: -m: multiple ppm at a time
  * flag==2: -p: parallel ppm processing
  * flag==3: -s: show a single ppm
  * flag==0: like assignment 1
  * If flag is of 10th digit, it has the same flag as first digit
  * 	, but specifically for unhide
  * 
  * NOTE: Don't need to check input file name with flag==1 or 2 since it is input
  *  	  as ordinary text. Will check again when open the files.
  * */
int checkArg2 (int flag, char *argv[]);


/* Function: changeNameNum
 * ---------------------
 * change file name numbering to i
 * 
 * i is between 0 to 254 (can have between 1 to 255 ppm pictures)
 * 
 * return void
 * (only possible error is if i>999. But this is tested with checkArg2.
 * May be an issue if use this function outside this assignment though.)
 */
void changeNameNum(char *name, int i);

#endif 
