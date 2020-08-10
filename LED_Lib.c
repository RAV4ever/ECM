#include <xc.h>                             //include xc.h library
#include "LCD_Lib.h"                        //include library with LCD functions
#include "USART.h"                          //include library with USART functions

#define _XTAL_FREQ 8000000                  //Set clock frequency to 8MHz.

void LEDshow(char input)
{
    LATA|=(0b00111111&input);               //Write 6 LSB of input to RA0-5 
    LATE|=0b0111&(input>>6);                //Write 3MSB of input to RE0-2
}

void LEDone(char input)
{
    LATA|=(0b00111111&(1<<(input-1)));      //HELP HERE WHAT IS THIS??????????
    LATE|=0b0111&(1<<(input-7));
}

void LEDclear(void)
{
    LATA&=0b11000000;                      //Set RA7&6 to 1 and all else to 0
    LATE&=0b1000;                          //Set RE3 to 1 and all else to 0 
}


