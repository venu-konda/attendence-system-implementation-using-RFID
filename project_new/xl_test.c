/*main.c: Very simple program to test the serial port. Expects the port to be looped back to itself */


#include<errno.h>
#include<stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include<unistd.h>
#include "xl.h"
int i,j=0;
char s[100];
int main()
{
	int fd ;
	int tx,rx ;

	puts("Opening serial port\n");

	if ((fd = serialOpen ("/dev/ttyUSB0",9600)) < 0)
	{

		fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
		return 1 ;
	}

	puts("serial port is opened\n");

	usleep(1000000);
//	writeMenu();
	
	while(1)
	{

		rx=serialGetchar(fd);
		fflush(stdout);
	     printf("%c",rx);	
	/*	if(rx==',')
		{
			printf("s==%s\n",s);
			s[i]='\0';
	//		writefile(s);
	//		bzero(s,100);
			i=0;
		}*/
	/*	else if((rx=='\r')||(rx=='\n'))
		{
			writefile("\n");
		}*/
	//	else
		s[i++]=rx;
	       if(rx=='\n' || rx == '\r')
	       { 
			s[i]='\0';
		       i=0;

		       writefile(s);
	//		system("clear");
	       }

//		printf("s = %s\n",s);
	}

       
       // printf("s=%s\n",s);
	usleep(100000);
//	system("clear");
	printf ("\n") ;
	serialClose(fd);
	puts("Closing serial port\n");
	return 0 ;
}
