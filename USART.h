/*configura el USART*/
void configUSART();

/*retorna el primer element del buffer*/
char TX1front(void);

/*posa en el buffer el valor que volem escriure per la virtual terminal*/
void sendString(char str[]);

/*mira si es produeix un error de frames*/
void error_ferr();

/*mira si es produeix un error de overrun*/
void error_oerr();

/*envia a la virtual terminal els nostres noms i el nom de la practica*/
void USARTsend();