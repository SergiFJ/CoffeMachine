# CoffeMachine
My CI subject project

WORK COFFEE MACHINE UPC FIB 2023-2024 Names: Sergi Font, Marc Estañol

Sidenote: This project was written in C language.

IMPORTANT NOTE: To run this program the lastest version of the "PROTEUS" simulator is needed.

OPERATION: The operation of our coffee machine consists of three selectors. Each option has three possibilities:

For temperature values: Calent, Tebi, and Fred.
For coffee type values: Llarg, Normal, and Curt.
For dose values: Fort, Normal, and Suau. 

To change the intensities of each option, pressing the 's' key decreases it, and with the 'w' key, it increases. To know which option is being addressed, it will be underline. You can switch between options with the 'a' and 'd' keys to go left and right, respectively.
Once the coffee configuration is chosen, pressing the 's' key or the "SELECT" button will start the coffee preparation. The preparation time will depend on the coffee configuration (as indicated in the practice statement). When finished, it will be indicated that the coffee is ready to drink, and another request can be made.

CODE STRUCTURE: Our code is structured into 16 files:

main.c: Main functions of the code as well as the description of global variables.
GLCD.c, .h, ASCII.h, splash.h, config.h: Functions given by the statement.
pantalla.c, .h: Contains all functions that "draw" on the GLCD screen.
PWM.c, .h: Contains all functions that involves the PWM
USART.c, .h: Contains all functions that involves the USART
ADC.c, .h: Contains all functions that involves the ADC
TMR.c, .h: Contains all functions that involves the timers
