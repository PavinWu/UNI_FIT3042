#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/*
int testPass(char *b){
	*b = "yeahmate";
	return 0;
}
*/

int main () {
	
	//int *ip = NULL;
	//if (ip != NULL) (*ip)+=2;
	
	//printf("int: %d \n", *ip);
	//return 0;
	
	/**/
	//char a[] = "adsdfasdf";
	
	//int i = 0;
	//while (a[i] != '\0') {
		//i++;
	//}
	//printf("%d", i);
	
	/**/
	/*
	char s1[] = "ab";
	char s2[] = "ad";
	printf("%d", strcmp(s1, s2));
	*/
	
	/**/
	/*
	FILE *fp;
	
	fp = fopen("test.txt", "w");
	char s1[] = "0123456789abc";
	char s2[20];
	strncpy(s2, s1[9], 3);
	*(s2+3) = '\0';
	fprintf(fp, "%s", s2);
	fclose(fp);
	
	* */
	
	/**/
	//char c = 89;
	//printf("%c", c);
	
	/**/
	FILE *f;
	//FILE *g;
	
	f = fopen("Asg1/cat-04.ppm", "r+b");
	////g = fopen("yeah.ppm", "wb");
	
	//char buffer[3]; //one pixel <- 3 shades -> 3 bytes (24 bits)
	
	//fread(buffer, 3, 1, f);
	//printf("%u ", buffer[0]);
	////fwrite(buffer, 3, 1, g);
	
	//fclose(f);
	////fclose(g);
	
	//how to read for all? What happens at end of file?
	
	/*
	char buffer[300];
	while(!feof(f)){
		fread(buffer, 3, 100, f);
		int i;
		for (i=0; i < 300; i++)
			printf("%u ", buffer[i]);
	}
	*/
	
	/**/
	//int i=0;
	//while(i<\
	//5) {\
		//printf("%d", i);
		//i++;
	//}
	
	
	/**/
	/*
	char abc[20] = "abc\0";
	printf("%d", strcmp("abc", abc));
	*/
	
	/**/
	/*
	char abc[6] = "1h2jj4";
	printf("%d\n", atoi(abc));
	*/
	
	/**/
	/*
	char buffer[21];
	fgets(buffer, 20, stdin); //how to do with pointers?
	printf("%s\n", buffer);
	*/
	
	/**/
	//int x=4324;
	//int y=456;
	//float *a=10.0;
	//float *z = (float) x/y;	//need (float) to not round down
	//printf("%f", modff(*z, *a));
	////if (z%10.0f > 0) {
		////z = (int) z+1;
	////}
	
	
	/**/ //round
	//float a = 112312.832;
	//int a_int = (int) a;
	//float a_test = a-0.5;
	//if (a_test>=a_int) {	//should be just ==
		//printf("round up");
	//}
	//else{
		//printf("round down");
	//}
	
	/**/ //round up only
	/*
	float a = 112312;
	//float a = 112312.123123;
	int a_int = (int) a;
	if (a==a_int) {
		printf("not round");
	}
	else if (a>=a_int){
		printf("round up");
	}
	* */
	
	
	/**/
	/*
	char * a = "adsadfadsf";
	testPass(a);
	printf("%s\n", a);
	*/
	
	/*
	char str[6] = {'a','b','\0'};
	int size = strlen(str);
	printf("%d\n", size);
	*/
	
	/**/
	/*
	int i;
	printf("%d\n", (int) sizeof(i));
	*/
	
	/**/
	int i[8];
	//i[]={0,0,0,0,0,0,0,0};
	
	/**/
	printf("%c\n", 123);
	////////////////////////////////////////////////////
	return 0;
}


