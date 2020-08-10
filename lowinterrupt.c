//#include<xc.h>
//
//volatile int risingRight,risingLeft;
//
//
//
//void __interrupt(low_priority) InterruptHandlerLow(void)
//{
//    if (PIR1bits.CCP1IF){
//        if (PORTCbits.RC2==1){
//            //capture
//            CCP1CON=0b00000100;
//            risingRight = ((CCPR1H<<8)|CCPR1L);
//            risingLeft = ((CCPR2H<<8)|CCPR2L);
//        }
//        else {
//            TMR1L=0;
//            TMR1H=0;
//            CCP1CON=0b00000101;
//        }
//        PIR1bits.CCP1IF=0;
//    }
//        
//    }
