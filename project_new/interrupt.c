#include<LPC21xx.h>
#include<stdlib.h>
#include "types.h"
#include "delay.h"
#include "pin_function_defines.h"

#include "lcd_defines.h"


#include "defines.h"
#include "kpm.h"
#include "lcd.h"

u32 readnum(void);
void eint1_isr(void) __irq
{
	int choice,num;
	IOCLR1=0XFF<<16;
	Initkpm();
Label:  CmdLCD(0x01);
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD("1.HOUR  2.MIN  3.SEC");
	CmdLCD(GOTO_LINE2_POS0);	
	StrLCD("4.DATE  5.MONTH");
	CmdLCD(GOTO_LINE3_POS0);
	StrLCD("6.YEAR 7.DAY");
	CmdLCD(GOTO_LINE4_POS0);
	StrLCD("ENTER CHOICE=");		
	choice=readnum();
	CmdLCD(0x01);
	switch(choice)
	{
		case 1:StrLCD("enter HOURS:");
		       num=readnum();
		      // while(ColScan() == 0);
		       CmdLCD(0x01);
		       if(num>=0&&num<24)
		       {
			       HOUR=num;
			       StrLCD("hour:");
			       U32LCD(HOUR);
			       delay_ms(500);
			       CmdLCD(0x01);
		       }
		       else
		       {
			       StrLCD("INVALID INPUT");
				   delay_ms(500);
			       CmdLCD(0x01);
			       goto Label;
		       }
		       break;
		case 2:StrLCD("enter MIN:");
		       num=readnum();
		       //while(ColScan() == 0);
		       CmdLCD(0x01);
		       if(num>=0&&num<60)
		       {
			       MIN=num;
			       StrLCD("minute:");
			       U32LCD(MIN);
			       delay_ms(500);
			       CmdLCD(0x01);
		       }
		       else
		       {
			       StrLCD("INVALID INPUT");
				   delay_ms(500);
			       CmdLCD(0x01);
			       goto Label;
		       }
		       break;
		case 3:StrLCD("enter SEC:");
		       num=readnum();
		      // while(ColScan() == 0);
		       CmdLCD(0x01);
		       if(num>=0&&num<60)
		       {
			       SEC=num;
			       StrLCD("seconds:");
			       U32LCD(SEC);
			       delay_ms(500);
			       CmdLCD(0x01);
		       }
		       else
		       {
			       StrLCD("INVALID INPUT");
				   delay_ms(500);
			       CmdLCD(0x01);
			       goto Label;
		       }
		       break;
		case 4:StrLCD("enter date:");
		       num=readnum();
		      // while(ColScan() == 0);
		       CmdLCD(0x01);
		       if(num>0&&num<=31)
		       {
			       DOM=num;
			       StrLCD("Date:");
			       U32LCD(DOM);
			       delay_ms(500);
			       CmdLCD(0x01);
		       }
		       else
		       {
			       StrLCD("INVALID INPUT");
				   delay_ms(500);
			       CmdLCD(0x01);
			       goto Label;
		       }
		       break;
		case 5:StrLCD("enter month:");
		       num=readnum();
		       //while(ColScan() == 0);
		       CmdLCD(0x01);
		       if(num>0&&num<=12)
		       {
			       MONTH=num;
			       StrLCD("month:");
			       U32LCD(MONTH);
			       delay_ms(500);
			       CmdLCD(0x01);
		       }
		       else
		       {
			       StrLCD("INVALID INPUT");
				   delay_ms(500);
			       CmdLCD(0x01);
			       goto Label;
		       }
		       break;
		case 6:StrLCD("enter year:");
		       YEAR=readnum();
		       //while(ColScan() == 0);
		       CmdLCD(0x01);
		       StrLCD("year:");
		       U32LCD(YEAR);
		       delay_ms(500);
		       CmdLCD(0x01);
		       break;
		case 7:	CmdLCD(GOTO_LINE1_POS0);
				StrLCD("0.SUN 1.MON ");	
				CmdLCD(GOTO_LINE2_POS0);
				StrLCD("2.TUE 3.WED");
				CmdLCD(GOTO_LINE3_POS0);
				StrLCD("4.THU  5.FRI");
				CmdLCD(GOTO_LINE4_POS0);
				StrLCD("6.SAT  ");
				StrLCD("enter :");
		       num=readnum();
			   if(num>=0&&num<=6)
			   {
			   DOW=num;
		       //while(ColScan() == 0);
		       CmdLCD(0x01);
		       StrLCD("DAY:");
		       U32LCD(DOW);
		       delay_ms(500);
		       CmdLCD(0x01);
			   }
			   else
			   {
			   		StrLCD("INVALID INPUT");
				   delay_ms(500);
			       CmdLCD(0x01);
			       goto Label;
			   }
		       break;
		default:StrLCD("Invalid Entry");
				delay_ms(500);
			     CmdLCD(0x01);
				  goto Label;
	}

	/*

	  HOUR=22;
	  MIN=22;
	  SEC=22;*/
	//  VICIntEnClr=1<<16;
	EXTINT=1<<2;
	VICVectAddr=0;
}
u32 readnum()
{
	int sum=0,n;
	while(1)
	{
		n=KeyScan();
		while(ColScan()==0);
		delay_ms(100);
		if(n<10)
		{
		//	CmdLCD(0x01);
			U32LCD(n);
			sum=(sum*10)+n;
		}
		else
		{
			break;
		}
	} 

	/*	while((n=KeyScan())<=9)

		{

		CmdLCD(0x01);

		U32LCD(n);

		sum=(sum*10)+n;

		}  */

	

		CmdLCD(0x01);

		//U32LCD(sum);
		return sum;
}
void Enable_EINT0(void)
{
	CFGPIN(PINSEL0,7,3);
	//VICIntSelect=0;
	VICIntEnable=1<<16;
	VICVectCntl1=0x20|16;
	VICVectAddr1=(u32)eint1_isr;
	EXTINT=1<<2;
	EXTMODE=1<<2;
	//StrLCD("WAITING FOR INPUT:");
	//EXTPOLAR=0;
}

