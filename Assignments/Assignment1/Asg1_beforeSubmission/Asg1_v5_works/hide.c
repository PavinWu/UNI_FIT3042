#include <stdio.h>
#include <string.h>
#include <stdlib.h>
 
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
		int fileType_len = strlen(fileName)-i-1;	//take away name, dot
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

int main(int argc, char *argv[]) {
/* argv[1]: input file name (*.ppm). argv[2]: output file name
 * Output has same name as input (for now)
*/ 
	
	// Check arguments
	if (argc > 3) {
		printf("More than 2 arguments. Take only the first two\n");
	}
	else if (argc < 3) {
		return awMan(1);
	}
	
	// Check file name
	int nameValidity = checkFileName(argv[1]);
	if (nameValidity != 0) {
		return nameValidity;
	}
	
	// Read file
	FILE *fin = fopen(argv[1], "rb");
	
	if (fin == NULL) {
		return awMan(4);
	}
	
	// check headers
	int width, height, color, notPass;
	char buffer;			//longest num posible is 65535 color space (TO BE USED LATER TOO!!!)
	notPass = headerChecking(fin, &width, &height, &color, &buffer);	//buffer is now first color, first pixel
	if(!notPass){
		printf("No errors in header\n");
		printf("%d, %d, %d\n", width, height, color);
	}
	else{
		return awMan(notPass);
	}
	
	//Write header to output
	FILE *fout = fopen(argv[2], "wb");
	char headerBuff[7] = {'P', '6', '\n', '\0'};	//mainBuffer is size 6 (5+'\0') + one \n
	int len;
	
	int i;
	for(i=0; i<4; i++) {
		switch (i) {
			case 0:
				break;
			case 1:
				sprintf(headerBuff, "%i\n", width);
				break;
			case 2:
				sprintf(headerBuff, "%i\n", height);
				break;
			case 3:
				sprintf(headerBuff, "%i\n", color);
				break;
		}
		len = strlen(headerBuff);
		fwrite(&headerBuff, len, 1, fout);
	}
	
	// Prep Take message
	float maxChar = (float) (width*height)*3/8;
	int maxChar_int = (int) maxChar - 1;	//round down. -1: save for \0
	printf("Message must be no more than %d characters.\n", maxChar_int);
	printf("Input message of no more than 500 characters at a time.\n");
	
	int j;
	int continueMsg = 1;
	int totalMsgLoop = 0;
	char bufferMsg[501];
	int binary[8] = {0,0,0,0,0,0,0,0};
	
	// Take message
	while (continueMsg && totalMsgLoop*500 < maxChar_int) {
		printf("Message: ");
		totalMsgLoop++;
		
		int charLastLoop = maxChar_int-totalMsgLoop*500-1;	//no. of char in last loop
		if (charLastLoop < 500) {		//last loop might allow only char less than 500
			fgets(bufferMsg, charLastLoop, stdin);
		}
		else if (charLastLoop < 0) {
			printf("Exceed characters limit");
		}
		else {
			fgets(bufferMsg, 500, stdin);
		}
		
		//convert message to binary. 
		for (i=0; i<strlen(bufferMsg); i++){
			//ASCII to binary
			if (bufferMsg[i] == '\0') {
				continue;
			}
			toBinary(bufferMsg[i], binary, 8);	//got binary of one character here => gotta get new byte from image 8 times
			
			/* //just to test
			
			printf("%i: ", bufferMsg[i]);
			for (j=0; j<8; j++){
				printf("%d ", binary[j]);
			}
			printf("\n");
			*/
			
			//edit it
			//if ASCII even: least sig dig is 0 => change only if message digit is 1
			//	vice versa for ASCII odd
			for (j=0; j<8; j++){
				if (buffer%2==0 && binary[j]==1) {
					buffer+=1;
				}
				else if ((buffer%2==1||buffer%2==-1) && binary[j]==0){
					buffer-=1;
				}
	
				fwrite(&buffer, 1, 1, fout);
				fread(&buffer, 1, 1, fin);	//Read ppm (edit before read: have left over byte)
			}
			//get -1 for 255 ascii !??
		}		
		
		// repeat
		printf("\nDo you wish to continue?(Y/N): ");
		int option = getchar();
		if (option==78 || option==110) {
			buffer = '\0';
			fwrite(&buffer, 1, 1, fout);
			fread(&buffer, 1, 1, fin);
			continueMsg = 0;
		}
		else {
			printf("\n");
		}
		while ((option = getchar()) != '\n' && option != EOF) { }	//flush stdin
	}
	//will not encounter feof up there (because of condition of while)
	//might encounter it down here
	while(!feof(fin)){
		fwrite(&buffer, 1, 1, fout);
		fread(&buffer, 1, 1, fin);
	}

	fclose(fin);
	fclose(fout);




	/*	//Not used
	//char * content = fgets(fp);
	
	//printf("%s", content);
	
	// Check input message
	
	
	// (need to be able to fit into the image)
	*/
	
	
	return 0;
}
