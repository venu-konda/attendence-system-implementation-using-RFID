#include<LPC21xx.h>
#include "uart0.h"
#include "delay.h"
#define SW 8
#define FOSC      12000000
#define CCLK      5*FOSC
#define PCLK      CCLK/4
#define PREINT_VAL ((PCLK/32768)-1)
#define PREFRAC_VAL (PCLK - ((PREINT_VAL + 1) * 32768))
void rtc_init()
{
        SEC=54; //Initialized seconds
        MIN=23; //Initialized minutes
        HOUR=07;//Initialized hour
		DOM=17;
		MONTH=05;
        YEAR=24;
		}
	
int main()
{		

		Enable_EINT0();
		Init_UART0();
		        //IODIR0=1<<SW;
  PREINT=PREINT_VAL;
  PREFRAC=PREFRAC_VAL;

  //AMR=0xFF; //mask all bits ie. no alarm

        rtc_init(); //initialize all seconds,minute and hour values
        CCR=0x01; //timer started
  while(1) //to keep alive so as to monitor the clock
  {
    UART0_Tx_char(HOUR/10+'0');
    UART0_Tx_char(HOUR%10+'0');
    UART0_Tx_char(':');
    UART0_Tx_char(MIN/10+'0');
    UART0_Tx_char(MIN%10+'0');
    UART0_Tx_char(':');
    UART0_Tx_char(SEC/10+'0');
    UART0_Tx_char(SEC%10+'0');
    UART0_Tx_str("\r\n");
    UART0_Tx_char(DOM/10+'0');
    UART0_Tx_char(DOM%10+'0');
    UART0_Tx_char('/');
    UART0_Tx_char(MONTH/10+'0');
    UART0_Tx_char(MONTH%10+'0');
    UART0_Tx_char('/');
    UART0_Tx_char(YEAR/10+'0');
    UART0_Tx_char(YEAR%10+'0');
    UART0_Tx_str("\r\n");
       delay_ms(1000);
	     }
}
