

/*
 * File:   UART_G6_v7.h
 * Author: Christian Mares y Manolo el del bombo
 *
 * Created on 1 de marzo de 2016, 9:55
 */

#include "UART_G6_v7.h"
#include "LCD_Explorer16_24H_G6_v7.h"
#include "Crono_G6_v7.h"
#include "CAD_G6_v7.h"
#include "PWM_G6_v7.h"
#include "SRF08_G6_v7.h"
#include "Main_G6_v7.h"


char ratzingerZ[4]={'0','0','0','0'};//buffer para guardar valores para asignar alservo
int fila=0,columna=0,fase_de_envio=0,fase_de_clrscr=0,Errores=0;
char caracter;

extern char hCmd;
extern unsigned char clear_screen;

// ================ Inicializa la UART2 Serial Port ======================
void initRS232_2( void){
// No requerido, lo hace el hardware
// UART2_TX_TRIS = 0;
// UART2_RX_TRIS = 1;
// UART2_TX_LAT = 1;
// UART2_RX_LAT = 1;
 U2MODE = 0; // 8bits, sin paridad, 1 stop, Uart parada
// _UARTEN_U2 =; // Habilita UART
// _USIDL_U2 =; // 0-> continua en modo Idle
// _IREN_U2 =; // Habilita IrDA
// _RTSMD_U2 =; // Modo pata U2RTS
// _UEN_U2 =; // Habilita U2TX, U2RX, U2CTS y U2RTS
// _WAKE_U2 =; // Habilita Wake-up al detectar Start en Sleep
// _LPBACK_U2 =; // Habilita modo Loopback
// _ABAUD_U2 =; // Habilita Auto-Baud
// _RXINV_U2 =; // Invierte polaridad para recepción
 _BRGH_U2 = 0; // BRGH
// _PDSEL_U2 =; // Paridad: 00= 8 bits sin paridad
// _STSEL_U2 =; // Duración bit Stop
 U2BRG = BAUD_RATEREG_2_BRGH0;
// U2BRG = 8; // Aplicando la formula da 7 y no va bien
 U2STA = 0;
// _UTXISEL_U2 =; // Tipo Interrupción Transmisión
// _UTXINV_U2 =; // Invierte polaridad pata transmisión
// _UTXBRK_U2 =; // Bit Breal
// _UTXEN_U2 =; // Habilita Transmisión
// _UTXBF_U2 =; // Estado Buffer transmisión (solo lectura)
// _TRMT_U2 =; // Estado Reg.Desplazamiento (solo lectura)
// _URXISEL_U2 =; // Tipo interrupción en recepción
 // 0x= interrupción con cada dato que llega
 // 10= interrupción a 3/4 del buffer
 // 11= interrupción cuando buffer lleno
// _ADDEN_U2 =; // Detección bit de dirección en 9 bits
// _RIDLE_U2 =; // Estado de la recepción (solo lectura)
// _PERR_U2 =; // Error de paridad /solo lectura)
// _FERR_U2 =; // Error de trama (solo lectura)
// _OERR_U2 =; // Error ocerrun (borrable/ solo lectura)
// _URXDA_U2 =; // 0=buffer vacío, 1= al menos un dato
 _U2TXIP = 2;
 _U2RXIF = 0; // Borra flag int. RX
 _U2TXIF = 0; // Borra flag int. TX
 _U2EIF = 0;  // Borra flag de Error UART
 _OERR_U2=0; // Según versión corrige bugg
 _UARTEN_U2 = 1; // Habilita la UART_2
 _UTXEN_U2 = 1; //Habilita transmisión,
 //Debe activarse después de habilitar UART
// RUTINA QUE ESPERA LA DURACION DE UN BIT ANTES DE EMPEZAR A TRANSMITIR
 int esp;
 for(esp=0;esp<5000;esp++)
//Estas dos lineas sirven para esperar la duración de un bit//
//      int i=0, it=(1/(Fcy))/(1/BAUDRATE2);               //
//      while(i< it){i++;}                                //
///////////////////////////////////////////////////////////

U2TXREG=0;

putRS232_2(0x1b);putRS232_2('[');putRS232_2('?');putRS232_2('2');putRS232_2('5');putRS232_2('l');
fila = 4;

 _U2TXIE = 1; // Habilita int. de transmisión
 _U2RXIE = 1;
 _U2EIE = 1; // Habilita Int_ de Error UART

 clear_screen=1;
} // Fin Inicialización UART 2

// =========================== FUNCIONES POR ENCUESTA ==========================
void putRS232_2( char c){
 while ( _UTXBF_U2 == 1); // Espera mientras buffer Tx está lleno
 U2TXREG = c; // Carga dato a transmitir

}

char getRS232_2( void){
 while ( !_URXDA_U2); // Espera a que llegue un dato
 return U2RXREG; // recoge el dato
}

//============================== RUTINAS DE ATENCION ===========================
void _ISR _U2TXInterrupt(void){
 _U2TXIF = 0;

 if(!clear_screen){
     if(fila<LINES){
         if(columna<=15){
            U2TXREG = get_char_ventana(fila,columna); // Envía dato de la posicion fila columna de ventana lcd
            columna++;
         }
         else if(fila<LINES-1){
             if(fase_de_envio==0){
                U2TXREG = CR;
                fase_de_envio++;
             }
             else {
                 U2TXREG = LF;
                 fila++;
                 columna=0;
                 fase_de_envio=0;
             }
         }
         else{
             U2TXREG=0;
             fila=LINES;
             columna=0;
         }
     }
     else if(fila == LINES){
         switch(fase_de_envio)
         {
             case 0:
                 U2TXREG = 0x1b;
                 fase_de_envio++;
                 break;
             case 1:
                 U2TXREG = '[';
                 fase_de_envio++;
                 break;
             case 2:
                 U2TXREG = 'H';
                 fase_de_envio = 0;
                 fila = 0;
                 break;
         }
     }
 }
 else {
     switch(fase_de_clrscr)
     {
         case 0:
             U2TXREG = 0x1b;
             fase_de_clrscr++;
             break;
         case 1:
             U2TXREG = '[';
             fase_de_clrscr++;
             break;
         case 2:
             U2TXREG = '2';
             fase_de_clrscr++;
             break;
         case 3:
             U2TXREG = 'J';
             fase_de_clrscr = 0;
             clear_screen=0;
             break;
     }

 }

// _U2TXIF = 0;
} // FIN _U2RXInterruptt
void _ISR _U2RXInterrupt(void){
    // enviar_comando(U2RXREG);
    hCmd = U2RXREG;
    _U2RXIF = 0;
}
void __attribute__((interrupt, no_auto_psv)) _U2ErrInterrupt (void){
 Nop();
 Nop();
 Errores++;
 _U2EIF = 0;
}

//============================ FUNCIONES ADICIONALES ===========================
//void enviar_comando(char c){
//    if (c>='0' && c<='9')
//        meterIntEnBuffer(c);
//    else if(c == '\n'){
//        set_position(1,getBuffer());
//        clear_buffer();
//    }
//    else if(c == 'e')
//        stop_Crono();
//    else if(c == 'r')
//        ini_Crono();
//    else if(c == 't')
//        pause_Crono();
//    else if(c == 'q'){
//        display_upper_line();
//    }
//    else if(c == 'a'){
//        display_lower_line();
//    }
//    else if(c == 'c'){
//        switch_estCalibrar();
//    }
//    else if(c == 'v')
//        calibrar_servo(1);
//    else if(c == 'j'){
//        girar_Izq(1);
//    }
//    else if(c == 'k'){
//        center_Servo(1);
//    }
//    else if(c == 'l'){
//        girar_Drch(1);
//    }
//    else if(c == 'z'){
//        read_dirI2C(0xE2);
//    }
//}

void meterIntEnBuffer(char c){
    ratzingerZ[0]=ratzingerZ[1];
    ratzingerZ[1]=ratzingerZ[2];
    ratzingerZ[2]=ratzingerZ[3];
    ratzingerZ[3]=c;
}
int getBuffer(){
    return (ratzingerZ[0]-48)*1000
          +(ratzingerZ[1]-48)*100
          +(ratzingerZ[2]-48)*10
          +(ratzingerZ[3]-48);
}
void clear_buffer(){
    ratzingerZ[0]='0';
    ratzingerZ[1]='0';
    ratzingerZ[2]='0';
    ratzingerZ[3]='0';
}