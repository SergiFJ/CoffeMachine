/* 
 * accions USART
 */

#include <xc.h>
#include <string.h>
#include <stdio.h>

#define _XTAL_FREQ 8000000  

char buff[64];
int iteration = 0;
int last_element = 0;


void configUSART(){
   SPBRG1 = 16;
   //enable transmitter
   TXSTA1bits.TXEN = 1;		//enable transmit
   TXSTA1bits.TX9 = 0; 		//8 bits
   TXSTA1bits.SYNC = 0;		//asyncron
   RCSTA1bits.SPEN = 1;		//serial port enable
   //enable receiver
   RCSTA1bits.CREN = 1;		//enable receiver
   //achieve baud rate
   BAUDCON1bits.BRG16 = 1;	//high baud rate
   TXSTA1bits.BRGH = 1;		//high mode
   
   RCSTA1bits.RX9 = 0;		//8 bits data receiving
   //interruptions
   RCIE = 1;
   RC1IE = 1;
   GIE = 1;
   PEIE = 1;
   IPEN = 1;
}


char TX1front(void) {
    if (iteration == last_element) return '\0';
    return buff[iteration++];
}


void sendString(char str[]) {
    TX1IE = 0;
    if (last_element == iteration) {
        int fi = strlen(str);
        for (int i = 0; i < fi; ++i) buff[i] = str[i];
        iteration = 0;
        last_element = strlen(str);
    } else {
        int fi = strlen(str) + last_element;
        for (int i = last_element; i < fi; ++i) buff[i] = str[i-last_element];
        last_element = fi;
    }
    TX1IE = 1;
}


void error_ferr(){
   if(RCSTAbits.FERR) {
      RCSTAbits.SPEN = 0;
      RCSTAbits.SPEN = 1;
   }
}	


void error_oerr(){
   if (RCSTAbits.OERR) {
      RCSTAbits.OERR = 0;
      RCSTAbits.OERR = 1;
      RCSTAbits.CREN = 0;
   }
}


void USARTsend(){
   sendString("L7 Projecte\r\nSergi Font Jane\r\nMarc Estanol Altadill\r\n");
}
