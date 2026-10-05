#include "types.h"

void delay_ms(int dlyMS)
{
  u32 j;
  for(;dlyMS>0;dlyMS--)
  {
   for(j=12000;j>0;j--);
  }
}
void delay_us(int dlyUS)
{
  u32 j;
  for(;dlyUS>0;dlyUS--)
  {
   for(j=12;j>0;j--);
  }
}
void delay_s(int dlyS)
{
  u32 j;
  for(;dlyS>0;dlyS--)
  {
   for(j=12000000;j>0;j--);
  }
}


