#include <stdio.h>
#include <unistd.h>	//fork,execv
#include <string.h>	//strcpy
#include <stdlib.h>	//malloc

#include <sys/types.h>	//waitpid
#include <sys/wait.h>

int main (int argc, char *argv[]){
	int status;
	pid_t pid = fork();
	if (pid) {
		waitpid(pid, &status, 0);
	}
	else {
		char **args = (char **) malloc(sizeof(char *)*(argc));
		args = argv+1;
		printf("%s\n", args[0]);
		//args+argc-1 = NULL;	#argv already null terminated.
		//how to put manually though?
		execv(args[0], args);
		free(args);
	}
	
	return 0;
}
