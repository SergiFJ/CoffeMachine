/* 
 * accions TMR
 */

#include <xc.h>
#include <string.h>
#include <stdio.h>

#define _XTAL_FREQ 8000000  


void configTimer(){
   T08BIT = 0;
   T0CS = 0;
   PSA = 0;
   T0CONbits.T0PS = 0b100;
   TMR0 = 3036; //carrega del timer
   TMR0IF = 0;
   TMR0IE = 1;
   GIE = 1;
   IPEN = 1;
   TMR0H = 1;
   TMR0ON = 0;
}