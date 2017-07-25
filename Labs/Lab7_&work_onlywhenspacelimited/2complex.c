#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>


int main (int argc, char *argv[]){
	char *paramStr = (char *) malloc(sizeof(char)*100);		//fixed size input
	fgets(paramStr, 100, stdin);
	char *tokBuff = strtok(paramStr, " ");
	
	char **params = (char **) malloc(sizeof(char *));
	int counter = 1;
	while (tokBuff != NULL){
		*(params+counter-1) = tokBuff;
		
		tokBuff = strtok(NULL, " ");
		counter++;
		params = (char **) realloc(params, sizeof(char *)*counter);
	}
	*(params+counter-1) = NULL;
	
	pid_t pid = fork();
	int status;
	if(pid){
		waitpid(pid, &status, 0);
	}
	else{
		execv(params[0], params);
	}
	

	
	/*while(counter > 0){	//strtok strings sit somewhere forbidden...
		//we also did not allocate space for these ourselves!
		//no need to deallocate
		free(*(params+counter-1));
		counter--;
	}*/
	free(params);	//resized params block, not the pointers in memory pointed by params block
	
	free(tokBuff);
	free(paramStr);
	
	return 0;
}