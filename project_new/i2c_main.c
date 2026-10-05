#include <LPC21xx.h>

#include "types.h"

#include "i2c.h"

#include "i2c_eeprom.h"

#include "delay.h"

#include <string.h>


#define I2C_EEPROM_SA 0x50 //7Bit Slave Addr

#define gLED 1<<16 //P1.16

#define rLED 1<<17 //P1.17


//u8 p __attribute__((at(0x40000040)))="";

u8 p;	

int main()

{

	 init_i2c();

	 IODIR1=gLED|rLED;

	

	 i2c_eeprom_write(I2C_EEPROM_SA,0x00,'k');

	 p=i2c_eeprom_read(I2C_EEPROM_SA,0x00);

	 delay_ms(100);

	 if(p=='o')

	 {

		 IOPIN1^=gLED;	 //p1.16

   }		

	 else

   {

		 IOPIN1^=rLED;	 //p1.17

   }

	 while(1);

}
