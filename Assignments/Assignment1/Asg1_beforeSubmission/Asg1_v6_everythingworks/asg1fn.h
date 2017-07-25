#include <stdio.h>	
#ifndef ASG1_H_	//guard?
#define ASG1_H_

int awMan(int awManCase);
int checkFileName(char fileName[]);
void skipWSorCom(char *ch, FILE *fp, int *skipped);
int isNumeric(char str[]);
int headerChecking(FILE *fp, int *width, int *height, int *color, char *buffOut);
void toBinary (int num, int binary[], int arrSize);

#endif
