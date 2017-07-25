/*
 * .h for Helper functions for hidev2.c (extendhidev2.h)
 * 
 * By:Pavin Wu (26916592)
 * Date: 24/4/17
 * */

#include <stdio.h>	
#ifndef EXTENDHIDE_H_	//guard?
#define EXTENDHIDE_H_

/* Function: writeHeader
 * ---------------------
 * write ppm header into fout
 * 
 * width: integer width of desired ppm image
 * height: integer height of desired ppm image
 * color: integer color depth of desired ppm image (between 1 to 2^16-1)
 * fout: file pointer to write the header to
 * 
 * return integer 4 if file open error, otherwise return 0.
 * Will always return 0 for the implementation in hide.c as fout is checked
 * before hand (just to be explicit).
 */ 
int writeHeader (int width, int height, int color, FILE *fout);


/* Function: changeLastBit
 * -----------------------
 * Change last bit of an 8bit character to the binary value.
 * 
 * buffer: pointer to the 8bit character whose last bit is to be changed
 * binary: value of buffer's new last bit 
 * 
 * return integer -1 if buffer points to NULL, otherwise returns 0.
 * Will not encounter this with the implementation in hide.c as *buffer was 
 * given a value after a successful headerChecking.
 */
int changeLastBit (char *buffer, int binary);


/* Function: embedBinary
 * -----------------------
 * embed each of 8bit binary into 8bytes from ppm
 * 
 * buffer: pointer to buffer of a single byte from ppm file. 
 * 	 	If not EOF first, will be changed after the first iteration of the loop.
 * binary: integer array containing the bits to embed
 * fin: pointer to ppm file to be read
 * fout: pointer to output ppm file
 * lastEmbedBin: a pointer to loop counter, used to specify the bit in binary
 * 		to be written. Will be non-0 if previous file terminates before all 8
 * 		bits were written
 * 
 * return integer 0 if there is no error, 
 * 				10 if ppm is truncated.
 * 
 * NOTE: Please check fin and fout not empty yourself!
 */
int embedBinary(char *buffer, int *binary, FILE *fin, FILE *fout, 
		int *lastEmbedBin);


/* Function: headerCheckingAll
 * ---------------------------
 * Check header for all [picnum] number of ppm.
 * 
 * width: pointer to int (array) to store width data of each ppm file obtained
 * 		during headerChecking. fin[i] will have width[i].
 * height: same as width, but for height.
 * color: same as width, but for color.
 * buffer: same as width, but of type char. We have this as we only know when
 * 		header terminates by reading until first binary bit of ppm (?). But this
 * 		first bit is a part of data we want, so we need to pass this on.
 * **WARNING** width, height, color and buffer needs to have the size of [picnum], 
 * 		or will cause memory overrun.
 * 
 * picnum: number of ppm picture to have header checked. If input has no flag, 
 *		use picnum==1. If has -m flag, input picnum the number-of-files.
 *		Other flags essentially behave as no flag for each ppm.
 * totSize: total possible message size that can be stored in ppms. Includes \0.
 * fin: array of file-pointers, for which the opened file with in the function
 * 		can later be used else where in the program
 * finname: starting filename whose header to be checked. If -m flag, will
 * 		be in format [filename]-000.ppm. Other flags will be [filename].ppm
 * opmode: operating mode for program. For meaning of each opmode, see the 
 * 		single digit flags on checkArg2 in asg2fn.c
 * 
 * return 0 if no errors, 4 if fin[i] is NULL, or error of headerChecking (from
 * 		asg1fn).
 */
int headerCheckingAll(int *width, int *height, int *color, char *buffer, 
			int picnum, int *totSize, FILE *fin[], char *finname, int opmode);


/* Function: readMsg
 * -----------------
 * read secret message from stdin, to be written in ppm
 * 
 * width: width information obtained from original ppm's header
 * height: height information obtained from original ppm's header
 * msg: pointer to the message to be read.
 * msgLen: pointer to size_t of the message written (+1 if need to include '\0'.
 * 		This "message" is what was read from fread. May be less than what's 
 * 		actually input through stdin!
 * 
 * return integer 0 if no error, 3 if malloc returns NULL pointer,
 */
 /*
 * NOTES: 
 *  - Do fread with maxChar_int-1 to as 
 * 		- maxChar_int is the only info we may deduce initially. If the ppm image
 * 		  is truncated, we can deal with it as we go.
 * 		- -1 to keep space for '\0'. Just so we can deal with the msg like a string
 *  - Put maxChar_int-1 as nmemb argument instead of size argument, since we 
 * 	  want to "count" based on the assumption that a unit value is of size 1.
 *	  This allows us to append '\0' successfully at the end. 	
 *  - Can support write message of upto 2^31/(8*3) total characters (or the integer
 *    counter will overflow and will not be able to keep track of the message.
 *    It is unlikely that user will provide a message that big, however (guess).
 */
int readMsg(int maxChar_int, char *msg, size_t *msgLen);


/* Function: writeMsg
 * ------------------
 * Write the secret message onto a copy of desired ppm. 
 * Similar to writeMsg for asg1, but this one allows not writing whole message
 * to one ppm file. Also allow to keep track of where message was last written.
 * These are done by passing values of pointers to next instance of writeMsg2,
 * had fin terminates before all bits in binary were written.
 * (This function is for one ppm only. If need to do with multiple ppms, 
 * use for loop with this function.) 
 * 
 * buffer: starting character to write into binary section of ppm
 * msg: char pointer to message to be written.
 * msgLen: size_t struct indicating the size of message to be written (excluding
 * 		'\0')
 * fin: pointer to original ppm file
 * fout: pointer to ppm file to write to
 * counter: pointer to integer indicating how much of msg is written.
 * lastEmbedBin: pointer to integer indicating the last index of binary array,
 * 		which its binary value was already written.
 * binary: binary array to write onto ppm.
 * 
 * return 0 if no error, 4 if file open error, else err of embedBinary().
 */
int writeMsg2(char *buffer, char *msg, size_t *msgLen, FILE *fin, FILE *fout,
				int *counter, int *lastEmbedBin, int *binary);


/* Function: makeNameop1
 * ---------------------
 * make starting input, output name if operation mode of program is 1 (flag -m).
 * i.e. append '-000.ppm\0' at the end.
 * 
 * finname: char pointer with [size of file name from argv] + 9.
 * 		The size of memory pointed to finname must be large enough for 
 * 		additional '-000.ppm\0' (9 additional characters).
 * foutname: same as finname, but for output file
 * argv: arguments containing initial finname, foutname to be appended
 * 		(without '-000.ppm\0').
 * 
 * return 0 if no error, 1 if arguments incomplete for operation mode 1.
 */
int makeNameop1(char *finname, char *foutname, char *argv[]);

#endif
