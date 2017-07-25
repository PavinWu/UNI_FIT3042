
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#define BUFFSIZE 120

int main (){

	char *buffer = (char *) malloc(sizeof(char)*BUFFSIZE);
	fread(buffer, 1, BUFFSIZE, stdin);
	char *params[] = {buffer,0}; 
	
	pid_t pid = fork();
	int status;
	
	if(pid){	//parent
		waitpid(pid, &status, 0);
	}
	else {
		execv(params[0], params); 
	}
	return 0;
}
