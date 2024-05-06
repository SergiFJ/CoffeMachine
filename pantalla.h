/*escriu a la posicio indicada per la pagina page i el byte y 
el text de s*/
void writeTxt(byte page, byte y, char * s);

/*dibuixa la cafetera inicial de la FIB*/
void cafetera();

/*dibuixa a la GLCD els nostres noms i el nom de la practica*/
void writeNames();

/*fa totes les crides dels dibuixos inicials de la GLCD abans d'arribar a la seleccio pel cafe*/
void GLCDinicial();

/*dibuixa la pantalla de seleccio del tipus de cafe*/
void GLCDseleccio();

/*segons en l'estat que ens trobem de la creacio del cafe, executa lo necessari*/
void estat_actual();