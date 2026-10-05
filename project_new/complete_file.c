#include <LPC21xx.H>  /* LPC21xx definitions */
#include <string.h>
#include "i2c.h"
#include "delay.h"
#include "i2c_eeprom.h"
#include "pin_function_defines.h"
#include"lcd.h"
#include"lcd_defines.h"

#define UART_INT_ENABLE 1
#define FOSC 12000000
#define CCLK 5*FOSC
#define PCLK CCLK/4
#define PREINT_VAL ((PCLK/32768)-1)
#define PREFRAC_VAL (PCLK - ((PREINT_VAL + 1) * 32768))

void InitUART0(void);  /* Initialize Serial Interface */
void UART0_Tx(char ch);
char UART0_Rx(void);
void UART0_isr(void) __irq;
void UART0_Str(char *s);
void UART0_Int(unsigned int n);
void UART0_Float(float f);
void DelayS(unsigned int dly);
void rtc_init(void);
void Enable_EINT0(void);
char buff[10],dummy;
unsigned char i = 0, ch, r_flag;

char id[3][9]={"12600717","00326553","00354554"};
char name[3][10]={"kala","harsha","sri"};
//char arr[10]="12638593";
							    
void switchcase(int n)
{
switch(n)
{
	case 0:UART0_Str("SUNDAY");
			break;
	case 1:UART0_Str("MONDAY");
			break;
	case 2:UART0_Str("TUESDAY");
			break;
	case 3:UART0_Str("WEDNESDAY");
			break;
	case 4:UART0_Str("THURSDAY");
			break;
	case 5:UART0_Str("FRIDAY");
			break;
	case 6:UART0_Str("SATURDAY");
			break;
}
}


void UART0_isr(void) __irq
{
	if ((U0IIR & 0x04)) // Check if receive interrupt
	{
		ch = U0RBR;  /* Read to Clear Receive Interrupt */
		if (ch != 0x03)
		{
			buff[i++] = ch;
		}
		else
		{
			buff[i] = '\0';
			i = 0;
			r_flag = 1;
		}
	}
	else
	{
		dummy = U0IIR; // Read to Clear transmit interrupt
	}
	VICVectAddr = 0; /* Dummy write */
}

void InitUART0(void) /* Initialize Serial Interface */
{
	CFGPIN(PINSEL0,0,1);
	CFGPIN(PINSEL0,1,1); /* Enable RxD0 and TxD0 */
	U0LCR = 0x83;         /* 8 bits, no Parity, 1 Stop bit */
	U0DLL = 97;           /* 9600 Baud Rate @ CCLK/4 VPB Clock */
	U0LCR = 0x03;         /* DLAB = 0 */

#if UART_INT_ENABLE > 0
	VICIntSelect = 0x00000000; // IRQ
	VICVectAddr0 = (unsigned)UART0_isr;
	VICVectCntl0 = 0x20 | 6; /* UART0 Interrupt */
	VICIntEnable = 1 << 6;   /* Enable UART0 Interrupt */
	U0IER = 0x03;       /* Enable UART0 RX and THRE Interrupts */
#endif
}

void UART0_Tx(char ch)  /* Write character to Serial Port */
{
	U0THR = ch;
	while (!(U0LSR & 0x20));
}

char UART0_Rx(void)    /* Read character from Serial Port */
{
	while (!(U0LSR & 0x01));
	return (U0RBR);
}

void UART0_Str(char *s)
{
	while (*s)
		UART0_Tx(*s++);
}

void UART0_Int(unsigned int n)
{
	unsigned char a[10] = {0};
	int i = 0;

	if (n == 0)
	{
		UART0_Tx('0');
		return;
	}
	else
	{
		while (n > 0)
		{
			a[i++] = (n % 10) + 48;
			n = n / 10;
		}
		--i;
		for (; i >= 0; i--)
		{
			UART0_Tx(a[i]);
		}
	}
}

void UART0_Float(float f)
{
	int x;
	float temp;
	x = f;
	UART0_Int(x);
	UART0_Tx('.');
	temp = (f - x) * 100;
	x = temp;
	UART0_Int(x);
}

void DelayS(unsigned int dly)
{
	unsigned int i;
	for (; dly > 0; dly--)
		for (i = 1200000; i > 0; i--);
}

void rtc_init()
{
	SEC = 57;  // Initialize seconds
	MIN = 59;  // Initialize minutes
	HOUR = 9;  // Initialize hour
	DOM = 15;  // Initialize day of month
	MONTH = 5; // Initialize month
	YEAR = 24; // Initialize year
	DOW = 5;

	PREINT = PREINT_VAL;
	PREFRAC = PREFRAC_VAL;
	CCR = 0x01;
}

void print_RTC()
{
	UART0_Tx(HOUR / 10 + '0');
	UART0_Tx(HOUR % 10 + '0');
	UART0_Tx(':');
	UART0_Tx(MIN / 10 + '0');
	UART0_Tx(MIN % 10 + '0');
	UART0_Tx(':');
	UART0_Tx(SEC / 10 + '0');
	UART0_Tx(SEC % 10 + '0');
	UART0_Str(",");

	UART0_Tx(DOM / 10 + '0');
	UART0_Tx(DOM % 10 + '0');
	UART0_Tx('/');
	UART0_Tx(MONTH / 10 + '0');
	UART0_Tx(MONTH % 10 + '0');
	UART0_Tx('/');
	UART0_Tx(YEAR / 10 + '0');
	UART0_Tx(YEAR % 10 + '0');
	UART0_Str(",");
	switchcase(DOW);
	UART0_Str("\r\n");
}

	 void Lcd()
{
    CharLCD(HOUR/10+'0');
    CharLCD(HOUR%10+'0');
    CharLCD(':');
    CharLCD(MIN/10+'0');
    CharLCD(MIN%10+'0');
    CharLCD(':');
    CharLCD(SEC/10+'0');
    CharLCD(SEC%10+'0');
    //strLCD("\r\n");
    CmdLCD(GOTO_LINE4_POS0);
    CharLCD(DOM/10+'0');
    CharLCD(DOM%10+'0');
    CharLCD('/');
    CharLCD(MONTH/10+'0');
    CharLCD(MONTH%10+'0');
    CharLCD('/');
    CharLCD(YEAR/10+'0');
    CharLCD(YEAR%10+'0');
   // UART0_Tx_str("\r\n");
//              delay_ms(1000);
}

/*
int main()
{
	InitUART0();
	rtc_init();
	while (1)
	{
		i = 0; r_flag = 0;
		while(r_flag == 0);
		UART0_Str(buff);
		UART0_Str("\r\n");
		if(strstr(buff,arr))
			UART0_Str("valid");
		else
			UART0_Str("invalid");
		UART0_Str("\r\n");
		print_RTC();
		//UART0_Str("\r\n");
		UART0_Float(245.21);
		UART0_Str("\r\n");
		DelayS(1);
	}
}
  */


int main()
{
	int u;

	char status=0;
	InitUART0();
	init_i2c();
	rtc_init();
	InitLCD();
	Enable_EINT0();
	while (1)
	{
		i = 0;
	    r_flag = 0;
	    StrLCD("WAITING FOR INPUT:");
		while(r_flag == 0);
		CmdLCD(0x01);
		for(u = 0; u < 3; u++)
		{
			if (strstr(buff,id[u]))
			{	
				
				status = i2c_eeprom_read(0x50, 0x0000 + u);
				if (status != 0 && status != 1)
				{
					status = 0;
				} 
				if (status == 0)
				{
					UART0_Str(name[u]);
					UART0_Str(",");
					UART0_Tx('"');
					UART0_Str(id[u]);
					UART0_Tx('"');
					UART0_Str(",");
					UART0_Str("IN");
					UART0_Str(",");
					print_RTC();
					StrLCD((s8 *)name[u]);
					CmdLCD(GOTO_LINE2_POS0);
					StrLCD("IN");
					CmdLCD(GOTO_LINE3_POS0);
					Lcd();
					delay_ms(1000);
					CmdLCD(0x01);
				//	status = 1;
					//UART0_Tx('\0');
					i2c_eeprom_write(0x50, 0x0000 + u,1);
					break;
				}
				if (status == 1)
				{
					UART0_Str(name[u]);
					UART0_Str(",");
					UART0_Tx('"');
					UART0_Str(id[u]);
					UART0_Tx('"');
					UART0_Str(",");
					UART0_Str("OUT");
					UART0_Str(",");
					print_RTC();
					StrLCD((s8 *)name[u]);
					CmdLCD(GOTO_LINE2_POS0);
					StrLCD("OUT");
					CmdLCD(GOTO_LINE3_POS0);
					Lcd();
					delay_ms(1000);
					CmdLCD(0x01);

					
					
					i2c_eeprom_write(0x50, 0x0000 + u,0);
					break;
				}
			}
		}

		if(u == 3)

		{

			StrLCD("invalid");
			delay_ms(1000);
			CmdLCD(0x01);



		}
	}
} 
/*

int main()

{

	char status=0;
	InitUART0();
	init_i2c();

	UART0_Tx('B');

	i2c_eeprom_write(0x50, 0x00,'A');

	status = i2c_eeprom_read(0x50,0x00);

	UART0_Tx(status);

}	 */

  


