
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include <string.h>
#define BUFFSIZE 120
#define PARAM_SIZE 10

int main (){

	int i = 0; char *token = NULL;
	
	char *buffer = (char *) malloc(sizeof(char)*BUFFSIZE);
	fread(buffer, 1, BUFFSIZE, stdin);
	char **params = NULL;
	
	/* MIGHT BE WRONG!!!!!!
	//params = (char *) malloc(sizeof(char));	//allocate pointers to strings (i.e. no. of pointers to char)
		//param: pointers to strings
		//i.e. *params[] = {"..0","..1","..2"};
		//	this one means A pointer to (a pointer of "..0")
		//	1D string array
	//**params = (char **) malloc(sizeof(char*));	//pointer to a string
		//*param: a pointer to a string
		//*((*param)+i): ith char of that string
		//**no need to allocated for strtok**
	*/
	
	token = strtok(buffer, " ");
	while (token != NULL){
		params = (char **) realloc(params, sizeof(char *)*(i+1));	
			//increase size of string array
		//*(params+i) = token;
		*(params+i) = (char *) malloc(sizeof(char)*(strlen(token)));
		strcpy(*(params+i), token);
		token = strtok(NULL, " ");	
		i++;
	}
	
	//char *params[] = {buffer,0}; 
	
	pid_t pid = fork();
	int status;
	
	if(pid){	//parent
		waitpid(pid, &status, 0);
	}
	else {
		//execv(params[0], params); 
		execv((*params)+0, params); 
	}
	
	i = 0;
	while(*(params+i) != NULL){
		free((*(params+i)));
		*(params+i)=NULL;	///////from gdb, params can only hold one char*
		///token WAS NEVER INITIALIZED!!!!
		///causing *(params+i) = token; to actually not contain any allocated mem
		i++;
	}
	free(params);
	free(buffer);
	free(token);
	return 0;
}
