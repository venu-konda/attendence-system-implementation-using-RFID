#include<stdio.h>
#include<stdlib.h>
void writeMenu(void)
{
        FILE *fp;
        fp=fopen("data.xls","w");
        if(fp == NULL)
        {
                printf("file not open\n");
                exit(0);
        }
        fprintf(fp,"%s,","s.no");
        fprintf(fp,"%s,","name");
        fprintf(fp,"%s,","id");
        fprintf(fp,"%s,","status");
        fprintf(fp,"%s\n","time");
}

