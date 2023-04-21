//UNIVERSIDAD DEL VALLE DE GUATEMALA
//IE2023 Programación de Microcontroladores
//Autor:		Mónica Alfaro
//Compilador:	pic-as (v2.36), MPLABX (v6.00)
//
//Programa:	POSTLAB8  (VENTANA HIPERTERMINAL CASOS (LEER POT Y MOSTRAR ASCII )	
//
//				
//Dispositivo:	PIC16F887
//Hardware:	LEDs en el puerto D, botones en el puerto B
//
//Creado:	       19 de abril , 2023
//Última modificación:   19 de abril , 2023




#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits pin
#pragma config WDTE = OFF       // Watchdog Timer Enable bit 
#pragma config PWRTE = OFF      // Power-up Timer Enable bit 
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit 
#pragma config CP = OFF         // Code Protection bit 
#pragma config CPD = OFF        // Data Code Protection bit 
#pragma config BOREN = OFF      // Brown Out Reset Selection bits 
#pragma config IESO = OFF       // Internal External Switchover bit 
#pragma config FCMEN = ON      // Fail-Safe Clock Monitor Enabled bit 
#pragma config LVP = ON         // Low Voltage Programming Enable bit 

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit 
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits 

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//Librerias para el pic y variables
#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ 8000000 //Frecuencia de 8MHz

//Funciones
void USART_Tx(char data);
char USART_Rx();
void USART_Cadena(char *str);

char valor; //variables

//Interrupciones
void main(void){
    //SETUP
    //No se utilizara pines analogicos
    ANSEL = 0X00;   
    ANSELH = 0x00;
    //Puertos A y B como salida
    TRISA = 0x00; 
    TRISB = 0x00; 
    PORTA = 0x00;
    PORTB = 0x00;
    
    OSCCONbits.IRCF = 0b111 ;  // config. de oscilador interno
    OSCCONbits.SCS = 1;         //reloj interno
    
    //UART
    //Confi. serial comunication
    TXSTAbits.SYNC = 0;    
    TXSTAbits.BRGH = 1;     
    BAUDCTLbits.BRG16 = 1;  
    //Velocidad de baud rates
    SPBRG = 207;                  
    SPBRGH = 0;             
    //Activar modulo serial
    RCSTAbits.SPEN = 1;     
    RCSTAbits.RX9 = 0;     
    RCSTAbits.CREN = 1;     
    TXSTAbits.TXEN = 1;     
    
//LOOP PRINCIPAL
    while (1) {
        //se muestra menu
        USART_Cadena("\r Selecciona una opcion \r");
        USART_Cadena(" 1) Leer potenciometro \r");
        USART_Cadena(" 2) Enviar ASCII \r \r");
        
        //Leyendo para recibir caracter
        while(PIR1bits.RCIF == 0);  
        //Se guardara en valor
        valor = USART_Rx();
          
        switch(valor){
            case ('1'):
                USART_Cadena(" Hola \r");
                break;
                        
            case ('2'):
                USART_Cadena(" Ingresa un caracter  ");
                while(PIR1bits.RCIF == 0);
                //Se representa en el puerto B
                PORTB = USART_Rx();  
                USART_Cadena("\r Hecho \r");
                break;
                        
            case ('3'):
                USART_Cadena(" Inválido ");
                
                break;
        }
    }
    
    return;   
}

/*funciones*/
    void USART_Tx(char data){
        while(TXSTAbits.TRMT == 0);
        TXREG = data;
    }

    char USART_Rx(){
        return RCREG; 
       }
    //str, leer cuando es un texto
    void USART_Cadena(char *str){
        while(*str != '\0'){
            USART_Tx(*str);
            str++;
        }
    }