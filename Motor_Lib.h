/* 
 * File:   Motor_Lib.h
 * Author: Nick
 *
 * Created on November 21, 2019, 11:05 PM
 */

#ifndef MOTOR_LIB_H
#define	MOTOR_LIB_H

struct DC_motor                         //define structure for the DC motors
{                               
        char power;                     //motor power percentage        
        char direction;                 //motor spin direction
        unsigned char *dutyLowByte;     //motor PWM duty cycle low bit location
        unsigned char *dutyHighByte;    //motor PWM duty cycle high bit location
        char dir_pin;                   //Set the direction pin 
        int PWMperiod;                  //Set the PWM period, based on resolution calculations.
};
void MotorSetup(void);
void setMotorPWM(struct DC_motor *m);
void turnLeft(struct DC_motor *m_L, struct DC_motor *m_R);
void turnRight(struct DC_motor *m_L, struct DC_motor *m_R);
void halt(struct DC_motor *m_L, struct DC_motor *m_R);
void FullSpeedAhead(struct DC_motor *m_L, struct DC_motor *m_R);
void FullSpeedReverse(struct DC_motor *m_L, struct DC_motor *m_R);

#endif

