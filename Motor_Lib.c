#include<xc.h>

#pragma config OSC = IRCIO                      //Internal oscillator
#define _XTAL_FREQ 8000000                      //Set clock frequency to 8MHz

/*
 * -------------------------
 * Motor Control Library file
 * 
 * Made by Nikolaos Kalogeropoulos and Kyriakos Theocharides
 * 
 * For the ME3-Embedded C for Microcontrollers course
 * Imperial College London
 * Fall 2019
 * 
 * ---------------------------
 * 
 * Short purpose: Set the PWM module and control the motors through PWM.
 * 
 * Known Bugs:
 * 
 * ---------------------------
 * The following functions set the PWM module of the PIC to work with the motors
 * on the robot base. PWM Outputs and mode outputs go from the PIC to the L293D
 * motor controller chip. 
 * 
 *                               _________
 *                  |     ?-----|-2    15-|-----------?
 *              RB4 |     |     |  L293D  |           |
 *              RB3-|-----?     |         |           |
 *      PIC     RB2-|-----------|?-?7? ? ? ? ?1?0?-?|--?        |
 *              RB1-|------------------------?        |
 *              RB0-|---------------------------------?
 *              Vdd |
 *                  |
 * 
 * 
 * ------------------------------
 
 * 
 * 
 *  
 * 
 * 
 * 
 * 
 * 
 *  
 */


struct DC_motor                         //define structure for the DC motors
{                               
        char power;                     //motor power percentage        
        char direction;                 //motor spin direction
        unsigned char *dutyLowByte;     //motor PWM duty cycle low bit location
        unsigned char *dutyHighByte;    //motor PWM duty cycle high bit location
        char dir_pin;                   //Set the direction pin 
        int PWMperiod;                  //Set the PWM period, based on resolution calculations.
};

void MotorSetup(void)
{
    PDC0L=0;                            //Lower PWM duty cycle register set to 0 (For right motor)
    PDC0H=0;                            //Higher PWM duty cycle register set to 0 (For right motor)
    PDC1L=0;                            //Lower PWM duty cycle register set to 0 (For left motor)
    PDC1H=0;                            //Higher PWM duty cycle register set to 0 (For left motor)
    
    PTCON0 = 0b00000000;                //free running mode, 1:1 prescale
    PTCON1 = 0b10000000;                //enable PWM timer

    PWMCON0 = 0b01100111;               //PWM1/3 enabled, all independent mode
    PWMCON1 = 0x00;                     //special features, all 0 (default)

    PTPERL =  0b11000111;               //base PWM period low byte  ->  PWM set to 100us
    PTPERH =  0b00000000;               //base PWM period high byte
}

void setMotorPWM(struct DC_motor *m)                                        //NICK EXPLAIN HERE PLZ
{
    int PWMduty;                                                            //Define PWMduty to store duty cycle time
    
    if ((m->direction)){                                                    //If direction == 1 (forward)
        PWMduty=m->PWMperiod-((int)(m->power)*(m->PWMperiod))/100;          //Set duty cycle time according to PWM period and requested power
    }
    else {                                                                  //If direction == 0 (backward)
        PWMduty=((int)(m->power)*(m->PWMperiod))/100;                       //Set duty cycle time according to PWM period and requested power
    }
    
    PWMduty = PWMduty << 2;                             //set temporary on time with the two last bits shifted
    
    *(m->dutyLowByte)= PWMduty & 0b11111111;            //send to low byte time register
    *(m->dutyHighByte)= (PWMduty>>8) & 0b00111111;      //send to low byte time register
    
    if (m->direction){
        LATB |= 1<<(m->dir_pin);                        //set direction pin
    }
    else {
        LATB &= ~(1<<(m->dir_pin));                     //set direction pin
    }
}

void turnLeft(struct DC_motor *m_L, struct DC_motor *m_R)                   //Turn left function
{
        
    m_L->power--;                                                           //Set Left motor power to 50%
    m_R->power++;                                                           //Set Right motor power to 50%
    
    m_R->direction = 0;                                                     //Set Right direction to reverse
    m_L->direction = 1;                                                     //Set Left direction to forward
    
    setMotorPWM(m_R);                                                       //Set duty cycles to perform requested movement
    setMotorPWM(m_L);
}

void turnRight(struct DC_motor *m_L, struct DC_motor *m_R)
{
    m_L->power++;                                                           //Set Left motor power to 50%
    m_R->power--;                                                           //Set Right motor power to 50%
    
    m_R->direction = 1;                                                     //Set Right direction to forward
    m_L->direction = 0;                                                     //Set Left direction to reverse
    
    setMotorPWM(m_R);                                                       //Set duty cycles to perform requested movement
    setMotorPWM(m_L);
}

void halt(struct DC_motor *m_L, struct DC_motor *m_R)
{
    m_R->direction = 1;                                                     //Set Right direction to forward     
    m_L->direction = 1;                                                     //Set Left direction to forward   
    
    m_R->power = 20;                                                        //Set power to 30% -> Equivalent to OFF (works for some reason)
    m_L->power = 20;                                                        //Set power to 30% -> Equivalent to OFF (works for some reason)
    
    setMotorPWM(m_R);                                                       //Set duty cycles to perform requested movement              
    setMotorPWM(m_L);
}

void FullSpeedAhead(struct DC_motor *m_L, struct DC_motor *m_R)
{

    m_R->direction = 0;                                                     //Set Right direction to backward           
    m_L->direction = 0;                                                     //Set Left direction to backward
    
    //Accelerate slowly up to half power so it is not instantaneous
    for ((m_L->power) ;(m_L->power)<50;(m_L->power)++){                     //From current power value until 50%
        m_R->power=m_L->power;                                              //Set right and left motor powers to be equal
        setMotorPWM(m_L);                                                   //Write to left motor 
        setMotorPWM(m_R);                                                   //Write to right motor 
        __delay_ms(10);                                                     //Delay so it is not instantaneous
    }
    __delay_ms(5);
}

void FullSpeedReverse(struct DC_motor *m_L, struct DC_motor *m_R)
{

    m_R->direction = 1;                                                     //Set Right direction to backward           
    m_L->direction = 1;                                                     //Set Left direction to backward
    
    //Accelerate slowly so it is not instantaneous
    for ((m_L->power) ;(m_L->power)<50;(m_L->power)++){                     //From current power value until 50%     
        m_R->power=m_L->power;                                              //Set right and left motor powers to be equal
        setMotorPWM(m_L);                                                   //Write to left motor
        setMotorPWM(m_R);                                                   //Write to right motor
        __delay_ms(10);                                                     //Delay so it is not instantaneous
    }
    __delay_ms(5);
}