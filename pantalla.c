/* 
 * accions pantalla
 */

#include <xc.h>
#include <string.h>
#include <stdio.h>
#include "ADC.h"
#include "PWM.h"
#include "GLCD.h"
#include "splash.h"
#include "USART.h"

#define _XTAL_FREQ 8000000  

const char * s1 = "L7 Projecte\r\n";
const char * sergi = "Sergi Font Jane\r\n";
const char * marc = "Marc Estanol Altadill\r\n";

char tassa1[17] = {0x3E,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0x24,0x24,0x18};
char tassa2[10] = {0x01,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x01,0x01};
char cafe[9] = {0x7F, 0x81, 0x81, 0x81, 0x81, 0x81, 0x7F, 0x42, 0x3C};
char humo[9] = {0x00, 0x50, 0x28, 0x00, 0x50, 0x28, 0x00, 0x00, 0x00};
char humo2[9] = {0x00, 0x28, 0x50, 0x00, 0x28, 0x50, 0x00, 0x00, 0x00};

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


state_t estat = 0;	//estat amb el que es troba la seleccio del cafe
char c = 'n';		//valor que entra ja sigui pels botons o pel USART
int value_aigua = 15;	//valor de l'aigua que triem al fer el cafe
int value_molinet = 10;	//valor del molinet que triem al fer el cafe
int temps = 0;		//temps que tarda a fer-se el cafe
char impres = 0;	//boolea per veure si s'ha impres la alerta de poca aigua
char impres2 = 0;	//boolea per veure si s'ha impres la alerta de que no hi ha aigua


extern int value_temperatura;
extern int dutycycle;
extern int value;
extern alerta alerta_aigua;


void writeTxt(byte page, byte y, char * s) {
   int i=0;
   while (*s!='\n' && *s!='\0' && *s!='\r') 
   {
      putchGLCD(page, y+i, *(s++));
      i++;
   };
}


void cafetera() {
   for (byte p = 0; p < 8; ++p) 
      for (byte y = 0; y < 128; ++y) 
	 writeByte(p,y,bitmap[p*128+y]); 
}


void writeNames(){
   writeTxt(0,0,s1);
   writeTxt(3,5,sergi);
   writeTxt(4,2,marc);
}


void GLCDinicial(){
   GLCDinit();		   //Inicialitzem la pantalla
   clearGLCD(0,7,0,127);   //Esborrem pantalla
   setStartLine(0);        //Definim linia d'inici
   cafetera();
   __delay_ms(2000);
   USARTsend();
   clearGLCD(0,7,0,127);
   writeNames();
   __delay_ms(2000);
   clearGLCD(0,7,0,127);
}


void GLCDseleccio(){
   if (alerta_aigua != 2){
      writeTxt(0,0,"Dosi\n");
      for (int i = 0; i < 20; ++i) SetDot(8,0+i);
      writeTxt(2,0,"Fort\n");
      writeTxt(4,0,"Normal\n");
      writeTxt(6,0,"Suau\n");
      for (int i = 0; i < 30; ++i) SetDot(40,0+i);

      writeTxt(0,9,"Cafe\n");
      writeTxt(2,9,"Llarg\n");
      writeTxt(4,9,"Normal\n");
      writeTxt(6,9,"Curt\n");
      for (int i = 0; i < 30; ++i) SetDot(40,45+i);

      writeTxt(0,18,"Temp\n");
      writeTxt(2,18,"Calent\n");
      writeTxt(4,18,"Tebi\n");
      writeTxt(6,18,"Fred\n");
      for (int i = 0; i < 20; ++i) SetDot(40,90+i);
   }
}


void estat_actual(){
   //seleccio estats
   if (c == 's'){
      estat = 3;
      c = 'n';
   }
   if (c == 'a' && estat < 3){
      for (int i = 0; i < 20; ++i) ClearDot(8,45*estat+i);
      --estat;
      if (estat == 0xFF) estat = 2;
      for (int i = 0; i < 20; ++i) SetDot(8,45*estat+i);
      c = 'n';
   }
   if (c == 'd' && estat < 3){
      for (int i = 0; i < 20; ++i) ClearDot(8,45*estat+i);
      ++estat;
      if (estat > 2) estat = 0;
      for (int i = 0; i < 20; ++i) SetDot(8,45*estat+i);
      c = 'n';
   }
   
   //accio per estats
   if (estat == 0){
      if (c == 'w' && value_molinet < 15){
	 value_molinet += 5;
	 c = 'n';
	 if (value_molinet == 15){
	    for (int i = 0; i < 30; ++i) ClearDot(40,0+i);
	    for (int i = 0; i < 20; ++i) SetDot(24,0+i);
	 }
	 else if (value_molinet == 10){
	    for (int i = 0; i < 22; ++i) ClearDot(56,0+i);
	    for (int i = 0; i < 30; ++i) SetDot(40,0+i);
	 }
      }
      if (c == 'x' && value_molinet > 5){
	 value_molinet -= 5;
	 c = 'n';
	 if (value_molinet == 5){
	    for (int i = 0; i < 30; ++i) ClearDot(40,0+i);
	    for (int i = 0; i < 22; ++i) SetDot(56,0+i);
	 }
	 else if (value_molinet == 10){
	    for (int i = 0; i < 22; ++i) ClearDot(24,0+i);
	    for (int i = 0; i < 30; ++i) SetDot(40,0+i);
	 }
      }
   }
   if (estat == 1){
      if (c == 'w' && value_aigua < 20){
	 value_aigua += 5;
	 c = 'n';
	 if (value_aigua == 20){
	    for (int i = 0; i < 30; ++i) ClearDot(40,45+i);
	    for (int i = 0; i < 25; ++i) SetDot(24,45+i); 
	 }
	 else if (value_aigua == 15){
	    for (int i = 0; i < 20; ++i) ClearDot(56,45+i);
	    for (int i = 0; i < 30; ++i) SetDot(40,45+i);
	 }
      }
      if (c == 'x' && value_aigua > 10){
	 value_aigua -= 5;
	 c = 'n';
	 if (value_aigua == 10){
	    for (int i = 0; i < 30; ++i) ClearDot(40,45+i);
	    for (int i = 0; i < 20; ++i) SetDot(56,45+i);
	 }
	 else if (value_aigua == 15){
	    for (int i = 0; i < 25; ++i) ClearDot(24,45+i);
	    for (int i = 0; i < 30; ++i) SetDot(40,45+i);
	 }
      }
   }
   if (estat == 2){
      if (c == 'w' && value_temperatura < 70){
	 value_temperatura += 10;
	 c = 'n';
	 if (value_temperatura == 60){
	    for (int i = 0; i < 20; ++i) ClearDot(56,90+i);
	    for (int i = 0; i < 20; ++i) SetDot(40,90+i);
	 }
	 else if (value_temperatura == 70){
	    for (int i = 0; i < 20; ++i) ClearDot(40,90+i);
	    for (int i = 0; i < 30; ++i) SetDot(24,90+i); 
	 }
      }
      if (c == 'x' && value_temperatura > 50){
	 value_temperatura -= 10;
	 c = 'n';
	 if (value_temperatura == 60){
	    for (int i = 0; i < 30; ++i) ClearDot(24,90+i);
	    for (int i = 0; i < 20; ++i) SetDot(40,90+i);
	 }
	 else if (value_temperatura == 50){
	    for (int i = 0; i < 20; ++i) ClearDot(40,90+i);
	    for (int i = 0; i < 20; ++i) SetDot(56,90+i);
	 }
      }
   }
   
   if (estat == 3){    
      ADCON0bits.CHS = 0b00110;
      GODONE = 1;
      while(GODONE);
      calculs_temp();
      
      //animacio cafe fentse
      clearGLCD(0,7,0,127);
      writeTxt(0,0,"S'esta fent el teu cafe\n");
      for (int i = 0; i < 9; ++i) {
	 writeByte(4, 15+i, cafe[i]);
	 writeByte(3, 15+i, humo[i]);
	 writeByte(4, 57+i, cafe[i]);
	 writeByte(3, 57+i, humo[i]);
	 writeByte(4, 100+i, cafe[i]);
	 writeByte(3, 100+i, humo[i]);
      }
      
      temps = value_molinet + value_aigua;
      TMR0ON = 1;
      PORTAbits.RA0 = 1;
      while (temps != 0){
	 for (int i = 0; i < 9; ++i) {
	    writeByte(3, 15+i, humo2[i]);
	    writeByte(3, 57+i, humo2[i]);
	    writeByte(3, 100+i, humo2[i]);
	 }
	 if (temps == value_aigua){
	    formula();
	    PORTAbits.RA0 = 0;
	    PORTAbits.RA1 = 1;
	 }
	 __delay_ms(100);
	 for (int i = 0; i < 9; ++i) {
	    writeByte(3, 15+i, humo[i]);
	    writeByte(3, 57+i, humo[i]);
	    writeByte(3, 100+i, humo[i]);
	 }
	 __delay_ms(100);
      }
      PORTAbits.RA1 = 0;
      TMR0ON = 0;
      dutycycle = 0;
      formula();
      ++estat;
   }
   
   if (estat == 4){
      //animacio cafe acabat
      clearGLCD(0,7,0,127);
      
      writeTxt(0,0,"Agafa el teu cafe\n");
      
      for (int i = 0; i < 17; ++i){
	 writeByte(4,56+i,tassa1[i]);
      }
      for (int i = 0; i < 10; ++i){
	 writeByte(5,58+i,tassa2[i]);
      }
      //animacio cafe
      char i = 4;
      SetDot(23,61);
      writeByte(3,61,0xE3);
      SetDot(23,64);
      writeByte(3,64,0xE3);
      
      while (i){
	 writeByte(3,60,0x1C);
	 writeByte(3,63,0x1C);
	 __delay_ms(750);
	 clearGLCD(3,3,60,60);
	 clearGLCD(3,3,63,63);
	 
	 writeByte(3,65,0x1C);
	 writeByte(3,62,0x1C);
	 __delay_ms(750);
	 clearGLCD(3,3,65,65);
	 clearGLCD(3,3,62,62);
	 --i;
      }
      
      //reiniciar
      value_temperatura = 60;
      value_aigua = 15;
      value_molinet = 10;
      estat = 0;
      c = 'n';
      //torna a calcular l'estat de l'aigua
      GODONE = 1;
      ADCON0bits.CHS = 0b00111;
      while(GODONE);
      calculs_water();
      impres = 0;
      impres2 = 0;
      clearGLCD(0,0,0,127);
      clearGLCD(3,5,0,127);
      GLCDseleccio(); 
   }
}