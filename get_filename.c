#include "get_filename.h"

void get_filename(char *dest, char *src)
{
	char *p;
	int n = 0, num = 0;

	p = src;
	while (*p != '\0'){
		if (*p == '/')
			n++;
		p++;
	}
	for (p = src; *p != '\0'; p++){
		if (*p == '/'){
			num++;
		}
		if (n == num){
			strcpy(dest, p);
			return ;
		}
	}
}
