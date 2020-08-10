#include <xc.h>
#include<stdio.h>
#include "LCD_Lib.h"
#include "USART.h"

#define _XTAL_FREQ 8000000 

volatile char code[12];

void getRFID(void)//WORKS
{
    
    int leftCheck,rightCheck = 0; 
    
    SetLine(1);  
    LCD_String("Code: ");

    for (int i=0;i<10;i++){    
        SendLCD((code[i]|0b00110000),1);
        if ((code[i]>>6)==1){code[i]=(code[i]&0x0F)+9;}
        else{code[i]=code[i]&0x0F;}
    }

    SetLine(2);
    LCD_String("Checksum ");
    for (int i=10;i<12;i++){    
        SendLCD(code[i],1);
        if ((code[i]>>6)==1){code[i]=(code[i]&0x0F)+9;}
        else{code[i]=code[i]&0x0F;}
    }

    leftCheck=((code[0]<<4)|(code[1]&0x0F));
    for (int i=1;i<5;i++){
        rightCheck=((code[2*i]<<4)|code[2*i+1]);
        leftCheck^=rightCheck;
    }

    if (leftCheck==((code[10]<<4)|code[11]) && leftCheck!=0){
        LCD_String(" OK!");
    }
    else{
        LCD_String(" ERR");
    }
}

void IRsetup(void)//WORKS
{
    ANSEL0=0;
    ANSEL1=0;
    DFLTCON=0b01011000;
    CAP1CON = 0b00000110;               
    CAP2CON = 0b00000110;      
    
    QEICON=0;
    
   //xd

}

int readIR(void)
{
    unsigned int risingRight,risingLeft = 0;
    int diff=0;
    unsigned char buf[20];
    
    risingRight = ((CAP1BUFH<<8)|CAP1BUFL);             //Bit shift higher register to get complete value.
    risingLeft = ((CAP2BUFH<<8)|CAP2BUFL);              //Bit shift higher register to get complete value.   

    if (PIR3bits.IC1IF) PIR3bits.IC1IF = 0;
    if (PIR3bits.IC3DRIF) PIR3bits.IC3DRIF = 0;
    
    diff=risingRight-risingLeft;
        
    SetLine(1);
    sprintf(buf,"diff: %d",diff);
    LCD_String(buf);
    
//    SetLine(2);
//    sprintf(buf,"right: %u",risingRight);
//    LCD_String(buf);
    
    return diff;
    
}