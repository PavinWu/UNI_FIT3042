#ifndef NEWSTRING
#define NEWSTRING

typedef struct {
	int len;
	char *string;	//only keeps pointer => same size regardless of string size
} Newstring;

//same as strlen() from string.h, but with Newstring data type
int newstrlen(Newstring *str);

//output Newstring as str1 concatenated with str2
Newstring * newstrcat(Newstring *str1, Newstring *str2);

//output strdest as string pointed to strsrc???
Newstring * newstrcpy(Newstring *strdest, Newstring *strsrc);

#endif
