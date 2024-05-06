/* 
 * accions ADC
 */

#include <xc.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define _XTAL_FREQ 8000000  

typedef enum{
   hi_ha_aigua = 0,
   poca_aigua = 1,
   no_aigua = 2,
}alerta;

extern int dutycycle;


int value_temperatura = 60;	//valor de la temperatura de l'aigua que hem seleccionat
alerta alerta_aigua = 0;  	//alerta del nivell d'aigua que te la cafetera
float Vout = 0;		//valor que agafara el ADC


void configADC(){
   ADCON2bits.ADCS = 0b101;
   ADCON2bits.ACQT = 0b101;
   ADCON2bits.ADFM = 1;
   ADCON1bits.TRIGSEL = 0;
   ADCON1bits.PVCFG = 0;
   ADCON1bits.NVCFG = 0;
   ADCON0bits.ADON = 1;
   ADIF = 0;
   ADIE = 1;
   PEIE = 1;
   GIE = 1;
}


void calculs_temp(){
   //formula1
   int beta = 4050;
   int R2 = 4700;
   int T = beta/(log(-(R2-(R2*5.0)/Vout)/0.0059257))-273.2;
   //actualitzacio DC
   int diff = value_temperatura - T;
   if (diff <= -5){
      dutycycle = 100;
   }
   else if (diff >= 5){
      dutycycle = 0;
   }
   else {
      dutycycle = 50;
   }
}


void calculs_water(){
   if (Vout < 0.006) alerta_aigua = 2; //Perque el potenciometre el minim que indica es 0.005 i per aixi tenir el cas on no hi ha aigua
   else if (Vout < 1) alerta_aigua = 1;
   else alerta_aigua = 0;
}