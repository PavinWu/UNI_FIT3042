#include <stdio.h>

int main (void)
{
	int num = 0;
	int ch = getc(stdin);
 	
	while (ch != EOF)
	{
		if(ch != 10 && ch != 32){
			num++; //not count enter
		}
		ch = getc(stdin);
	}

	printf("Total Numbers of Characters is %d\n", num);
	return 0;
}
