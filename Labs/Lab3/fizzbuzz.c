#include <stdio.h>

int main ()
{
	#ifdef PINGPONG
	char fizz[] = "ping";
	char buzz[] = "pong";
	#else
	char fizz[] = "fizz";
	char buzz[] = "buzz";
	#endif
	
	int i;
	for (i=0; i<= 100; i++)
	{
		if(i%3 == 0 && i%5 == 0){
			printf("%s%s\n", fizz, buzz);	
		}
		else if(i%3 == 0){
			printf("%s\n", fizz);
		}
		else if(i%5 == 0){
			printf("%s\n", buzz);	
		}
		else{
			printf("%d\n",i);
		}
	}
	return 0;
}
