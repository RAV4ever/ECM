#include<xc.h>
#include "USART.h"
#include "LCD_Lib.h"
#include "LED_Lib.h"

volatile char code[12];
volatile char doneFlag;

char counter;
char indata;

void __interrupt(high_priority) InterruptHandlerHigh(void)//WORKS
{
    if (PIR1bits.RCIF){
        indata=getCharSerial();
        if (indata==0x03){
            doneFlag=1;
        }
        else if (indata==0x02){
            LATCbits.LATC4=1;
            counter=0;
        }
        else if (indata==0x0A || indata==0x0D){}
        else{
            code[counter]=indata;
            counter++;
            //SendLCD(indata,1);
        }
    }
}
