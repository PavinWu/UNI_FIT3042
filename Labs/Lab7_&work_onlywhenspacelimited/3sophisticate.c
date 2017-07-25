#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>


int main (int argc, char *argv[]){
	char *paramStr = (char *) malloc(sizeof(char)*1000);		//fixed size input
	fgets(paramStr, 1000, stdin);
	char *tokBuff = strtok(paramStr, " ");
	
	char **params = (char **) malloc(sizeof(char *));
	int counter = 1;
	char end = 0;
	while (tokBuff != NULL || !end){
		// only assign if it doesn't
		//	if it does, terminate current, do processing, then go to next command
		if(tokBuff != NULL && (strchr(tokBuff, '&') == NULL)){	//if no &. strchr needs to be later!!!
			*(params+counter-1) = tokBuff;
			tokBuff = strtok(NULL, " ");
		
			counter++;
			params = (char **) realloc(params, sizeof(char *)*counter);
		}
		else {	//if has & or done
			*(params+counter-1) = NULL;
	
			pid_t pid = fork();
			int status;
			if(pid){
				waitpid(pid, &status, WNOHANG);
				printf("Process: %d terminated with status %d\n", pid, status);
			}
			else{
				execv(params[0], params);
			}
			
			if (tokBuff != NULL){
				tokBuff = strtok(NULL, " ");	/////////////
				counter = 1;
				params = (char **) realloc(params, sizeof(char *));
				printf("\n");
			}
			else{
				end = 1;
			}
		}
	}
	
	
	free(params);	
	
	free(tokBuff);
	free(paramStr);
	
	return 0;
}