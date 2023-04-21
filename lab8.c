//UNIVERSIDAD DEL VALLE DE GUATEMALA
//IE2023 Programación de Microcontroladores
//Autor:		Mónica Alfaro
//Compilador:	pic-as (v2.36), MPLABX (v6.00)
//
//Programa:	LAB  (1 CADENA con comunicación serial)	
//
//				
//Dispositivo:	PIC16F887
//Hardware:	LEDs en el puerto D, botones en el puerto B
//
//Creado:	       16 de abril , 2023
//Última modificación:   16 de abril , 2023


// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits    
#pragma config WDTE = OFF       // Watchdog Timer Enable bit  
#pragma config PWRTE = OFF      // Power-up Timer Enable bit  
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit    
#pragma config CP = OFF         // Code Protection bit  
#pragma config CPD = OFF        // Data Code Protection bit  
#pragma config BOREN = OFF      // Brown Out Reset Selection bits  
#pragma config IESO = OFF       // Internal External Switchover bit  
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit 
#pragma config LVP = OFF         // Low Voltage Programming Enable bit 

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit 
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits 


#include <xc.h>
#include <stdio.h>

#define _XTAL_FREQ 4000000
#define BAUD_RATE 9600

/*                              PROTOTIPO                                     */
/*                              FUNCIONES                                     */

//Declaración de funciones 

void setup(void);           //Función I/O
void setup_UART(void);      //Función configurar comunicación serial

void SEND_CAR (char c) {    //Función enviar caracter - Transmisión-
    while(!TXIF);           //Mientras que el registro de TX esté vació
    TXREG = c;              //Envía caracter en puerto serial TX
}

void cadena(char *txt) {
    
    while (*txt != '\0') {  //Mientras pasa por la cadena
        SEND_CAR(*txt);     //Envía un caracter 
        txt++;              //Va pasando de caracter en caracter + + 
    }
}


void main() {
    
    setup();
    setup_UART();
    
    cadena("Hello mundo :P");
    /*                              LOOP                                      */

    while(1) {
    if (PIR1bits.RCIF) {//Revisa que hayan caracteres enviados
        char c = RCREG;      //Lectura caracter
        
        PORTD = 0;            //Limpiar puerto
        PORTD = c;            //Enviar caracter
    
        //Print caracter
        SEND_CAR(c);            //Envía el caracter especificado en el main
    }
}
}

/*                        FUNCIONES (SUBRUTINAS)                              */


void setup(void){
    ANSEL = 0;                 // Habilitar entrada analógica
    ANSELH = 0;
   
    TRISD = 0;                  //Declaración salidas
    PORTD = 0;

    
    OSCCONbits.IRCF = 0b110;   // Configuración del reloj interno con bits IRCF 
    OSCCONbits.SCS = 0b00;     
}

void setup_UART(void) {
    TXSTAbits.SYNC = 0;         // Comunicación serial modo asíncrona
    TXSTAbits.BRGH = 1;         // Selección velocidad alta baudios
    BAUDCTLbits.BRG16 = 0;      // Selección de 8 bits para control de baudios
    
    SPBRG = 25;                 // BAUDAJE 9600
    SPBRGH = 0;                 // Valor alto de registro de baudios
    
    RCSTAbits.SPEN = 1;         // Habilitar comunicación serial
    TXSTAbits.TX9 = 0;          // Tamaño de datos 8 bits (solo usamos 1 port)
    TXSTAbits.TXEN = 1;         // Habilitar transmisión de datos
    RCSTAbits.CREN = 1;         // Habilitar recepción de datos
    
    PIR1bits.RCIF = 1;          // Banderas para interrupción
}


