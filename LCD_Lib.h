/* 
 * File:   LCD_Lib.h
 * Author: Nikolaos Kalogeropoulos, Kyriakos Theocharides
 *
 * Created on November 21, 2019, 9:24 PM
 * 
 * This header file defines the functions needed for the LCD screen to be 
 * initiated and to function.
 */

#ifndef LCD_LIB_H
#define	LCD_LIB_H

void LCDTog(void);                                  //Toggle the send bit
void LCDout(unsigned char nib);                     //Send a single nib of data
void SendLCD(unsigned char Byte, char type);        //Send a whole byte of data
void LCDsetup(void);                                //Initiate the LCD screen
void SetLine (char line);                           //Set the LCD display line
void LCD_String(char *string);                      //Send a whole string
void LCDclear(void);                                //Clear Display

#endif

