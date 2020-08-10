#include<xc.h>
#include<stdio.h>

#pragma config OSC = IRCIO              //Internal oscillator
#define _XTAL_FREQ 8000000              //Set clock frequency to 8MHz

/*-----------------------------
 * USART Library file
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
 * Known Bugs: Cannot handle recieving more than a byte at a time from the PC, 
 *      though it is not for sure whether that is the main code's fault or the
 *      library's fault.
 *             Sometimes from the RealView terminal, it takes many presses of
 *      the send button to send a command.
 * 
 * ---------------------------
 * The purpose of this Library is to set the USART module commands. It comprises
 * of three functions: One to get a signal from the serial connection, one to 
 * send a signal from the PIC to the PC, and one to send a string from the PIC 
 * to the PC.
 * 
 * The functions work as such:
 * 
 * getCharSerial: Function that waits for the Receive Flag (RCIF) to drop and 
 *      then reads the data (a byte) from the RCREG register. When it is read, the flag 
 *      rises again until new data is in the register. Every time the register 
 *      has new data in, the flag is dropped and the process repeats.
 * 
 * sendCharSerial: Function that sends a byte of data through the serial port.
 *      It first wait for the TXIF flag to drop, so that the previous data has 
 *      been sent out and new ones will not overwrite the old ones (they will, 
 *      but not from the view of the receiver). The data to be sent are written
 *      to the TXREG register and then sent.
 * 
 * sendStringSerial: Function to send an entire string through the serial port. 
 *      It uses a pointer to point to the memory location of the data that the 
 *      function is called to display. Inside the function there is a while loop 
 *      with the condition to run while the 
 *      pointer value does not point to a null value, hence indicating that the 
 *      string has been completed. If that condition is satisfied then the 
 *      sendCharSerial function is called to send the dereferenced pointer value as a 
 *      display command, then increase the pointer value, and check again, until
 *      the null value is reached. Then a new line command and a reset cursor 
 *      are sent to begin in a new line in the terminal.
 * 
 *      (The above explanation has been taken from the LCD_String explanation 
 *      since the two operate with the same principle)
 * 
 *      NOTE: To work in the main code, the text in the sendStringSerial 
 *      function has to be in double quotation marks ("), not singular (').
 * 
 * 
 */


//-----------------------RECEIVE SERIAL DATA----------------------------------
char getCharSerial(void)
{
    while(!PIR1bits.RCIF);              //Wait until the ready flag is down.
    return RCREG;                       //Read the RCREG register to get the data.
}

//-----------------------SEND SERIAL DATA-------------------------------------
void sendCharSerial(char charToSend)
{   
    while(!PIR1bits.TXIF);              //Wait until the previous data has been sent.
    TXREG=charToSend;                   //Write to the TXREG register to set the sent data.
}

//-----------------------SEND STRING THROUGH SERIAL----------------------------
void sendStringSerial(char *stringToSend)
{
    while (*stringToSend != 0){         //While the string pointer does not point to an empty memory cell
        sendCharSerial(*stringToSend++);//Use the sendCharSerial command to send the byte in the memory cell and then increase the pointer value.
    }
            
    sendCharSerial('\n');               //New line
    sendCharSerial('\r');               //Reset pointer
}

//-------------------------SETUP USART----------------------------------------
void USARTsetup(void)
{
    PIE1bits.RCIE=1;                    //Enables the EUSART to receive interrupt
    IPR1bits.RC1IP=1;                   //High priority for the EUSART receive interrupt priority bit
    
    SPBRG = 205;                        //Set Baud rate low byte to 205
    SPBRGH = 0;                         //Set Baud rate high byte to 0
    BAUDCONbits.BRG16=1;                //Enable 16-bit Baud rate generator
    TXSTAbits.BRGH=1;                   //High speed asynchronous bad rate
    RCSTAbits.CREN=1;                   //Continuous receive mode
    RCSTAbits.SPEN=1;                   //Enable serial port, other settings default
    TXSTAbits.TXEN=1;                   //Transmit enabled, other settings default
}
