/* Main.c
 * Processor: PIC18F45K22
 * Compiler:  MPLAB XC8
 */

#include <xc.h>
#include <string.h>
#include <stdio.h>
#include "config.h"
#include "GLCD.h"
#include "ascii.h"
#include "PWM.h"
#include "USART.h"
#include "ADC.h"
#include "TMR.h"
#include "pantalla.h"

#define _XTAL_FREQ 8000000

typedef enum{
   molinet = 0,
   aigua,
   temperatura,
   fent,
   acabat
}state_t;

typedef enum{
   hi_ha_aigua = 0,
   poca_aigua = 1,
   no_aigua = 2,
}alerta;


char senyal[15] = {0xC0, 0xE0, 0xB0, 0x98, 0x8C, 0x86, 0x83, 0xDD, 0x83, 0x86, 0x8C, 0x98, 0xB0, 0xE0, 0xC0};

extern char impres;
extern char impres2;
extern char c;
extern float Vout;
extern alerta alerta_aigua;
extern int temps;
extern int value_temperatura;
extern int value_aigua;
extern int value_molinet;
extern state_t estat;



struct flancs{
   int count;
   char flanc;
};

struct flancs RC0;	//flancs dels botons
struct flancs RC1;
struct flancs RC2;
struct flancs RC3;
struct flancs RC4;

/*inicialitza els flancs dels botons*/
void flancs_init(struct flancs* f){
   f->count = 0;
   f->flanc = 0;
}


void interrupt high_RSI(){
   if (RC1IE && RC1IF) {	//virtual --> PIC
     error_ferr();
     c = RCREG1;
     if (c == 'w') {
	 sendString("Up\r\n");
     } else if (c == 'a') {
	 sendString("Left\r\n");
     } else if (c == 'x') {
	 sendString("Down\r\n");
     } else if (c == 'd') {
	 sendString("Right\r\n");
     } else if (c == 's') {
	 sendString("Seleccionar\r\n");
     }
   }
   if (TX1IF && TX1IE){
     char b = TX1front();
     if (b != '\0') TXREG1 = b; // Still data to write
     else TX1IE = 0;
   }


   if (ADIF == 1 && ADIE == 1){
     Vout = (ADRES/1023.0)*5.0; //regla de tres
     ADIF = 0;
   }

   if (TMR0IF && TMR0IE){
      TMR0 = 3036;
      --temps;//REDUIR 1 SEGONS
      TMR0IF = 0;
   }
}

/*mira quin boto s'ha apretat*/
void botons(){
   if (PORTCbits.RC0 && RC0.count == 0 && RC0.flanc){
      RC0.count = 5;
      sendString("Up\r\n");
      c = 'w';
      RC0.flanc = 0;
   }
   else if (PORTCbits.RC1 && RC1.count == 0 && RC1.flanc){
      RC1.count = 5;
      sendString("Down\r\n");
      c = 'x';
      RC1.flanc = 0;
   }
   else if (PORTCbits.RC2 && RC2.count == 0 && RC2.flanc){
      RC2.count = 5;
      sendString("Right\r\n");
      c = 'd';
      RC2.flanc = 0;
   }
   else if (PORTCbits.RC3 && RC3.count == 0 && RC3.flanc){
      RC3.count = 5;
      sendString("Left\r\n");
      c = 'a';
      RC3.flanc = 0;
   }
   else if (PORTCbits.RC4 && RC4.count == 0 && RC4.flanc){
      RC4.count = 5;
      sendString("Seleccionar\r\n");
      c = 's';
      RC4.flanc = 0;
   }
}

/*configura el PIC*/
void configPIC(){
   ANSELA=0x00;
   ANSELB=0x00;
   ANSELC=0x00;
   ANSELD=0x00;
   ANSELE=0x06;

   TRISA=0x00;
   TRISB=0x00;
   TRISC=0xDF;
   TRISD=0x00;
   TRISE=0x06;

   PORTD=0x00;
   PORTB=0x00;
}


void main(void)
{
   flancs_init(&RC0);
   flancs_init(&RC1);
   flancs_init(&RC2);
   flancs_init(&RC3);
   flancs_init(&RC4);

   configPIC();
   configUSART();
   configTimer();
   configPWM();
   configADC();

   GLCDinicial();

   //mira nivell aigua
   GODONE = 1;
   ADCON0bits.CHS = 0b00111;
   while(GODONE);
   calculs_water();

   GLCDseleccio();

   while (1)
   {
      //mira nivell aigua perque el podem haver canviat en qualsevol moment
      GODONE = 1;
      while(GODONE);
      calculs_water();

      //si hi ha aigua
      if (alerta_aigua != 2){
	 if (impres2){
	    clearGLCD(4,4,0,127);
	    GLCDseleccio();
	    impres2 = 0;
	 }
	 if (alerta_aigua == 1 && !impres){
	    for (int i = 0; i < 15; ++i) {
	       writeByte(7, 113+i, senyal[i]);
	    }
	    impres = 1;
	 }
	 else if (alerta_aigua == 0 && impres){
	    clearGLCD(7,7,113,128);
	    impres = 0;
	 }

	 error_oerr();
	 if (PORTCbits.RC0 == 0) RC0.flanc = 1;
	 if (PORTCbits.RC1 == 0) RC1.flanc = 1;
	 if (PORTCbits.RC2 == 0) RC2.flanc = 1;
	 if (PORTCbits.RC3 == 0) RC3.flanc = 1;
	 if (PORTCbits.RC4 == 0) RC4.flanc = 1;
	 botons();
	 if (RC0.count > 0) --RC0.count;
	 if (RC1.count > 0) --RC1.count;
	 if (RC2.count > 0) --RC2.count;
	 if (RC3.count > 0) --RC3.count;
	 if (RC4.count > 0) --RC4.count;
	 estat_actual();
      }
      //si no hi ha aigua, no volem guardar el valor que enviem
      else if (alerta_aigua == 2){
	 c = 'n';
	 value_temperatura = 60;
	 value_aigua = 15;
	 value_molinet = 10;
	 estat = 0;
	 if (!impres2){
	    impres = 0;
	    clearGLCD(0,7,0,127);
	    writeTxt(4,5,"NO HI HA AIGUA\n");
	    __delay_ms(2000);
	    impres2 = 1;
	}
      }
   }
}
