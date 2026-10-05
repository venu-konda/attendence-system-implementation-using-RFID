#include<stdio.h>
#include<stdlib.h>
void writefile(char *s)
{
	FILE *fp;
	fp=fopen("data1.xls","a");
	if(fp == NULL)
	{
		printf("file not open\n");
		exit(0);
	}
//	printf("s=%s\n",s);
	if((s[0]=='\n')||(s[0]=='\r'))
	{
		fprintf(fp,"%c",'\n');
	}
	else
	{
		fprintf(fp,"%s,",s);
	}
	fclose(fp);
}
