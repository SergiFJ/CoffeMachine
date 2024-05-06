/* 
 * accions PWM
 */

#include <xc.h>
#include <string.h>
#include <stdio.h>

#define _XTAL_FREQ 8000000  

int dutycycle = 0;	//dutycycle seleccionat
int value = 0;		//valor que hem de passar al ventilador per treballar

void formula(){
   value = 10*dutycycle;
   CCP3CONbits.DC3B = value&3;  // Bits menys significatius del cicle de treball
   CCPR3L = value >> 2;    //Bits mes significatius
}


void configPWM() {
    PR2 = 249;               // Valor per aconseguir una freqüència (formula)
    T2CON = 2;   	// Preescalador 16, 2 o 3 configura a 16 bits. Es la config del CKPS
    T2CONbits.T2CKPS = 2;
    CCP3CONbits.CCP3M = 0b1100;   // Mode PWM
    formula();
    CCPTMRS0bits.C3TSEL = 0; 		// Si PWM -> treballa amb timer 2
    TMR2IF = 0;		//Flag
    TMR2ON = 1;		//Activar el timer
    TMR2 = 0;		//Valor del timer
}