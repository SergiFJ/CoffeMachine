Funcionament de la cafetera:

Amb els botons o l'EUSART es pot escollir el tipus de cafe desitjat. Amb els botons/USART esquerra/a i dreta/d es permet canviar l'apartat de configuració.
Amb els botons/USART amunt/w i abaix/x es permet canviar en cada apartat de configuració el que es vol escollir.
Amb el botó/USART seleccionar/s es guarda la configuració de cafe actual i es començar a preparar el cafè.

La nostra cafetera avisa amb un senyal a la cantonada inferior dreta quan la tensio del sensor de nivell d'aigua es inferior a 1V. En cas de que no hi hagi aigua no 
es podra accedir a la configuració del cafe i sortira una senyal a la pantalla de que no hi ha aigua.

Quan s'estigui fent el cafè s'encendran dos llums. El primer quan el cafè s'estigui molint i el segon quan s'estigui injectant l'aigua.

Exemple d'ús:

Volem un cafe fort, curt i calent.
- Premerem el botó amunt per escollir fort.
- Premerem el botó dreta per canviar a la configuració del cafè.
- Premerem el botó abaix per escollir curt.
- Premerem el botó dreta per canviar a la configuració de la temperatura.
- Premerem el botó amunt per escollir calent.
- Premerem el botó seleccionar per començar a fer el cafè.

---------------------------------------------------------------------------------------

Sortida digital del molinet de cafè: RA0.
Sortida digital de la bomba d’aigua: RA1.
Sortida digital del ventilador amb PWM: RE0.
Sensor de temperatura amb NTC com a la pràctica L6b connectat al pin AN6.
Sensor nivell aigua simulat amb un potenciòmetre de 1kΩ connectat entre 0V i 5V. Dipòsit
ple quan el sensor marca 5V. Dipòsit buit quan el sensor marca 0V. Alarma de baix nivell
d’aigua quan la tensió baixa de 1V.
Botó amunt: RC0.
Botó abaix: RC1.
Botó dreta: RC2.
Botó esquerra: RC3.
Botó selecció RC4
Comunicació USART línia sèrie Tx1: RC6
Comunicació USART línia sèrie Rx1: RC7