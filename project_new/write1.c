#include<stdio.h>
#include<stdlib.h>
void writefile(char *s)
{
	FILE *fp;
	int i;
	fp=fopen("data1.xls","a");
	if(fp == NULL)
	{
		printf("file not open\n");
		exit(0);
	}
	printf("s=%s\n",s);
	fprintf(fp,"%s",s);

	fclose(fp);
}
