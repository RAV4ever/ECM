#include <stdio.h>
#include <stdlib.h>
#include "LCD_Lib.h"                        //include library with LCD functions
#include "USART.h"                          //include library with USART functions
#include "Motor_Lib.h"                      //include library with motor commands

#define _XTAL_FREQ 8000000                  //Set clock frequency to 8MHz.
#pragma config OSC = IRCIO                  //Internal oscillator

void getRFID(void);                         //include function getRFID
void IRsetup(void);                         //include function IRsetup

int main(void) {

    OSCCON=0x72;                            //Set clock frequency to 8MHz (NEED TO REDEFINE???)           
    while(!OSCCONbits.IOFS);                //Wait for OSC to become stable
    
    T5CON=0b00010001;                       //Activate Timer5 with 1:4 prescaler
    PR5 = 0xffff;                           //Timer overflow at 0xffff (Max value for unsigned int)
    LATA=0;                                 //LAT for all A ports = 0
    LATB=0;                                 //LAT for all B ports = 0
    LATC=0;                                 //LAT for all C ports = 0
    LATD=0;                                 //LAT for all D ports = 0
    LATE=0;                                 //LAT for all E ports = 0
    
    TRISA=0b00001100;                       //RA2&3 as input. All else as output
    TRISB=0b11000000;                       //RB7&6 as input. All else as output
    TRISC=0b10001110;                       //RC1&2&3&7 as input. All else as output
    TRISD=0b00110000;                       //RD4&5 as input. All else as output
    TRISE=0b1000;                           //RE3 as input. All else as output
    
    
    LCDsetup();                             //Call function to setup LCD screen
//    USARTsetup();                           //Call function to setup USART 
    IRsetup();                              //Call function to setup IR sensors
//    MotorSetup();                           //Call function to setup motors
    
    
    while(1){
    
        int temp=readIR();                              
        
        if (temp >= 100){
            SetLine(1);  
            LCD_String("RIGHT");
        __delay_ms(100);
        }
        
        
        if (temp <= -100){
            SetLine(1);  
            LCD_String("LEFT");
        __delay_ms(100);
        }
        
        if (-100 <= temp <= 100){
            SetLine(1);  
            LCD_String("FWD");
        __delay_ms(100);
        }
    
        LCDclear();
    }
}

