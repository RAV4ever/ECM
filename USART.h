/* 
 * File:   USART.h
 * Author: Nick
 *
 * Created on November 21, 2019, 10:38 PM
 */

#ifndef USART_H
#define	USART_H


char getCharSerial(void);
void sendCharSerial(char charToSend);
void sendStringSerial(char *stringToSend);
void USARTsetup(void);

#endif	/* USART_H */

