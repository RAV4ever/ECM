#include <xc.h>
#include <string.h>

/*-----------------------------
 * LCD Library file
 * 
 * Made by Nikolaos Kalogeropoulos and Kyriakos Theocharides
 * 
 * For the ME3-Embedded C for Microcontrollers course
 * Imperial College London
 * Fall 2019
 * 
 * ---------------------------
 * 
 * Short purpose: Set the LCD and make it work
 * 
 * Known Bugs:
 * 
 * ---------------------------
 * The following functions are here to make the LCD work. Detailed descriptions
 * will be given after each function and command. It is working on 4-bit mode 
 * and will be set up as such, hence why 4 data bits have been wired. The LCD 
 * Wireup to the controller is done as such:
 * 
 *                |
 *                | RA7
 *      RS--------|-RA6
 *      TS--------|-RC0
 *           |----|-RC1
 *      DB4--| |--|-RC2
 *      DB5----|  | RC3
 *      DB6-------|-RD0
 *      DB7-------|-RD1
 *                ¯¯¯¯¯¯¯¯¯
 * RS: Bit to tell the LCD that the incoming data is a command (0) or data to 
 *      be displayed (1)
 * 
 * TS: (or E bit in datasheet) Toggle bit to send data (when bit value falls
 *      from High to Low.
 * 
 * DBx: Data bits
 * ----------------------------
 * 
 * First thing in the code is renaming some of the pins that operate the LCD. 
 * Specifically port A6 is the RS bit, that dictates whether the sent bytes and
 * nibs are display data or command data. Next the TS bit (called E bit in the 
 * datasheet) is sent, that acts as a switch whose falling edge signals send 
 * information (a single nib) to the LCD.
 * 
 * LCDTog: Dedicated function to toggle the TS bit from 1 to 0 to create a 
 *      rising edge and send a command to the LCD.
 * 
 * LCDout: Function to set the Data bits to be sent to the LCD. From the pinout
 *      above one can see that the nibble has to be sent as RD1 RD0 RC2 RC1 in 
 *      order of decreasing bit significance.
 * 
 * SendLCD: Evolution of the LCDout function that sends whole bytes at a time 
 *      instead of nibbles. To do that it sends the four most significant bits 
 *      and then the four least significant bits. At the same time one can set 
 *      whether the data sent is a command for the LCD or data to be displayed.
 *      
 * LCD_Init: Initiation script to start up and set up the LCD screen. The code
 *      starts the LCD, sets it up as a 4bit input, 7 pixel per character, 
 *      2 line mode. THe instructions are taken directly from the datasheet.
 * 
 * SetLine: The LCD screen has 2 distinct lines and the text does not 
 *      automatically go from the end of the first line to the next. Hence 
 *      this function sets the cursor of the LCD to either the top or the 
 *      bottom line if it gets 1 or 2 respectively.
 * 
 * LCD_String: Function to send an entire string to the LCD by repeatedly 
 *      calling the SendLCD command. It uses a pointer to point to the memory
 *      location of the data that the function is called to display. Inside the
 *      function there is a while loop with the condition to run while the 
 *      pointer value does not point to a null value, hence indicating that the 
 *      string has been completed. If that condition is satisfied then the 
 *      SendLCD string is called to send the dereferenced pointer value as a 
 *      display command, then increase the pointer value, and check again, until
 *      the null value is reached.
 * 
 * --------------------------
 * 
*/

//--------------------------DEFINITIONS---------------------------------------

#define _XTAL_FREQ 8000000                  //8Mhz Processor clock time (needed for the delay functions)

#define LCD_RS PORTAbits.RA6                //Rename the RS pin to LCD_RS
#define ToggleSend PORTCbits.RC0            //Rename the TS pin to ToggleSend
#define DB7 PORTDbits.RD1                   //Rename Port RD1 as DB7
#define DB6 PORTDbits.RD0                   //Rename Port RD0 as DB6
#define DB5 PORTDbits.RD2                   //Rename Port RD2 as DB5
#define DB4 PORTDbits.RD3                   //Rename Port RD3 as DB4

//-------------------------LCD TOGGLE------------------------------------------
void LCDTog(void)               
{
    ToggleSend=1;                           //Set the bit to 1    
    __delay_us(10);                         //5us delay
    ToggleSend=0;                           //Set the bit to 0, to create falling edge and send command
}

//-------------------------SEND NIBBLE TO LCD----------------------------------
void LCDout(unsigned char nib)
{
    DB7=nib>>3;
    DB6=(nib>>2)&0b01;
    DB5=(nib>>1)&0b001;
    DB4=nib&0b0001;
   
    LCDTog();                               //Toggle to send the data to the LCD
    
    __delay_us(5);                          //5us delay
}

//--------------------------SEND BYTE TO LCD-----------------------------------
void SendLCD(unsigned char Byte, char type)
{
    LCD_RS=type;                            //Set the data type (1 for display, 0 for command)
    LCDout(Byte>>4);                        //Send the four most significant bits first
    __delay_us(10);                         //Delay 10us
    LCDout(Byte&0b00001111);                //Send the four least significant bits
    __delay_us(50);                         //Delay 50us
}   

//--------------------------LCD INITIATE---------------------------------------
void LCDsetup(void)
{
    OSCCON=0b11110010;    
    while(!OSCCONbits.IOFS);
    
    DB7=0;        //Set RD0(DB6), RD1(DB7) as Low to begin with
    DB6=0;
    DB5=0;      //Set RC1(DB4), RC2(DB5) as Low to begin with and RC0(TS pin) as High 
    DB4=0;
    ToggleSend=0;          //Set the Toggle Send bit to 0  
    LCD_RS = 0;
    
    __delay_ms(20);         //Delay 20ms

    LCDout(0b00000011);             //send 0b0011 to screen
    __delay_ms(5);                  //Delay 5ms
    LCDout(0b00000011);             //send 0b0011 to screen
    __delay_us(200);                //Delay 200us
    LCDout(0b00000011);             //send 0b0011 to screen
    __delay_us(50);                 //Delay 50us
    
    LCDout(0b00000010);             //send 0b0010 to screen to set 4bit mode
    __delay_us(50);
    
    SendLCD(0b00101000,0);          //Set Function: 4 bit interface, 2 line display, 5x7 dots per icon
    __delay_us(50);                 //Delay 50us
    SendLCD(0b00001000,0);          //Display off
    __delay_us(50);                 //Delay 50us
    SendLCD(0b00000001,0);          //Display clear
    __delay_ms(5);                  //Delay 5ms
    SendLCD(0b00000110,0);          //Entry Mode Set: Cursor is increasing (Right-to-left text), Display is not shifted
    __delay_us(50);                 //Delay 50us
    SendLCD(0b00001110,0);          //Display on
    __delay_us(50);                 //Delay 50us
    
    LCD_RS = 1;
}

//--------------------------SET LCD LINE---------------------------------------
void SetLine (char line)
{
    if (line==1) {SendLCD(0x80,0);} //If the requested line is the top one (1), send a command via the SendLCD function to set the LCD to write to the first line
    else{SendLCD(0xC0,0);}          //If the requested line is not 1 it is expected to be 2, hence the command sent tells the LCD to write to the lower line.
    __delay_us(50);                 //Delay 50us
}

//--------------------------SEND STRING TO LCD---------------------------------
void LCD_String(char *string)
{
    while(*string != 0){            //While the string pointer does not point to an empty memory cell
        SendLCD(*string++,1);       //Use the SendLCD command to send the byte in the memory cell as a display command (hence the 1) and then increase the pointer value.
        __delay_us(50);             //Delay 50us
    }
}

void LCDclear(void)
{
    SendLCD(0b00000001,0);          //Display clear
}