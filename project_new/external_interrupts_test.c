#include<LPC21xx.h>
#include<stdlib.h>
#include "types.h"
#include "delay.h"
#include "pin_function_defines.h"
#include "defines.h"
#include"kpm.h"
u32 readnum(void);
void eint0_isr(void) __irq
{
    	Initkpm();
		delay_ms(100);
		HOUR=readnum();
        while(ColScan() == 0);
        MIN=readnum();
        while(ColScan() == 0);
		SEC=readnum();
        while(ColScan() == 0);
        DOM=readnum();
        while(ColScan() == 0);
        MONTH=readnum();
        while(ColScan() == 0);
        YEAR=readnum();
        while(ColScan() == 0);
	EXTINT=1<<1;
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
                        sum=sum*10+n;
                }
                else
                {
                        break;
                }
        }
                return sum;
}
void Enable_EINT0(void)
{
	CFGPIN(PINSEL0,14,2);
	//VICIntSelect=0;
	VICIntEnable=1<<15;
	VICVectCntl0=0x20|15;
	VICVectAddr0=(u32)eint0_isr;
	EXTINT=1<<1;
	EXTMODE=1<<1;
	//EXTPOLAR=0;
}
