#include<stdio.h>
#include<stdlib.h>
int  main()

{char s[]="abcd,\"00001\",out,12:11";
	FILE *fp;
	int i;
	fp=fopen("d.xls","a");
	if(fp == NULL)
	{
		printf("file not open\n");
		exit(0);
	}
	printf("s=%s\n",s);
	fprintf(fp,"%s",s);

	fclose(fp);
}
