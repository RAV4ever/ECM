#include <xc.h>                             //include xc.h library
#include <stdio.h>                          //include studio library
#include "LCD_Lib.h"                        //include library with LCD functions
#include "USART.h"                          //include library with USART functions
#include "Motor_Lib.h"                      //include library with motor commands
//#include "LED_Lib.h"

#define _XTAL_FREQ 8000000                  //Set clock frequency to 8MHz.
#pragma config OSC = IRCIO                  //Internal oscillator

void getRFID(void);                         //include function getRFID
void IRsetup(void);                         //include function IRsetup
//int readIR(void);
void __interrupt(high_priority) InterruptHandlerHigh(void);         //include high priority interrupt
void __interrupt(low_priority) InterruptHandlerLow(void);           //include low priority interrupt

volatile char code[12];                     //define volatile char 'code' for reading RFID
volatile char doneFlag;                     //define volatile flag for (??????)

void main(void){                            //Main function loop
    
    OSCCON=0x72;                            //Set clock frequency to 8MHz (NEED TO REDEFINE???)           
    while(!OSCCONbits.IOFS);                //Wait for OSC to become stable
    
    T0CONbits.TMR0ON=1;                     //Enable Timer0
    T0CONbits.T016BIT=1;                    //Timer0 configured to 8 bit
    T0CONbits.T0CS=0;                       //Use internal clock (Fosc/4)
    T0CONbits.PSA=1;                        //Timer0 prescaler disabled.
    T0CONbits.T0PS=0b111;                   //1:256 prescale value (not used as disabled above)
    
//    T5CONbits.RESEN=1;
//    T5CONbits.T5MOD=0;
//    T5CONbits.T5PS=0;
//    T5CONbits.TMR5CS=0;
//    T5CONbits.TMR5ON=1;
    
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
        
    INTCONbits.GIEH=1;                      //Enables all unmasked interrupts
    INTCONbits.PEIE=1;                      //Enables all unmasked peripheral interrupts (CORRECT????)
    
    LCDsetup();                             //Call function to setup LCD screen
    USARTsetup();                           //Call function to setup USART 
    IRsetup();                              //Call function to setup IR sensors
    MotorSetup();                           //Call function to setup motors
    
    //LCD_String("Waiting");
    
//    unsigned int stepsCountR=0;
//    unsigned char prevPinValueR=0;
//    unsigned int stepsCountL=0;
//    unsigned char prevPinValueL=0;
    
    while(1){                               //Change this for Nick
        
        //LEDone(1);
//        __delay_ms(30);
        
        if (doneFlag==1){                   //If ID12 senses the RFID
            LCDclear();                     //Call function to clear LCD
            getRFID();                      //Call function to get RFID
            doneFlag=0;                     //Reset flag for sensing RFID
            while(1);                       //
        }
        
        //LEDone(2);
//        __delay_ms(30);
        
//        if (PORTDbits.RD7!=prevPinValueR){
//            stepsCountR++;
//            prevPinValueR=PORTDbits.RD7;
//        }
//        if (PORTDbits.RD6!=prevPinValueL){
//            LEDclear();
//            stepsCountL++;
//            prevPinValueL=PORTDbits.RD7;
//        }

        int temp=readIR();                              
//        sprintf(buf,"%d",temp);
//        LCD_String(buf);
//        __delay_ms(150);
        
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
        
//        if (temp>=20){
//            LEDclear();
//            LEDone(6);
//            
//            turnRight(&motorL,&motorR);
//        }
//        else if(temp<20){
//            LEDclear();
//            LEDone(5);
//            turnLeft(&motorL,&motorR);
//        }
//        else{
//            LEDclear();
//            LEDone(6);
//            LEDone(5);
//            FullSpeedAhead(&motorL,&motorR);
//        }
//        
//        LEDone(3);
//        __delay_ms(50);
//        __delay_ms(30);
        LCDclear();
        
    }
    //LEDone(4);
}