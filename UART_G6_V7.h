/* 
 * File:   UART_G6_v7.h
 * Author: Christian Mares y Manolo el del bombo
 *
 * Created on 1 de marzo de 2016, 9:55
 */

#include "p24hj256gp610A.h"

//======================================================================
// timing and baud rate calculations
#define Fosc 80000000 //Frecuencia reloj CPU
#define Fcy Fosc/2
#define BAUDRATE2 19200 // baudios (bits/sg) de transmision
//#define BAUDRATE2 9600 // baudios (bits/sg) de transmision
#define BAUD_RATEREG_2_BRGH1 ((Fcy/BAUDRATE2)/4)-1 // Para BRGH = 1
#define BAUD_RATEREG_2_BRGH0 ((Fcy/BAUDRATE2)/16)-1 // Para BRGH = 0
// Reg/bits UART2 serial port
// U2MODE no definido en P24FJ12GA010A.h
#define _STSEL_U2 U2MODEbits.STSEL
#define _PDSEL0_U2 U2MODEbits.PDSEL0
#define _PDSEL1_U2 U2MODEbits.PDSEL1
#define _BRGH_U2 U2MODEbits.BRGH
#define _RXINV_U2 U2MODEbits.RXINV
#define _ABAUD_U2 U2MODEbits.ABAUD
#define _LPBACK_U2 U2MODEbits.LPBACK
#define _WAKE_U2 U2MODEbits.WAKE
#define _UEN0_U2 U2MODEbits.UEN0
#define _UEN1_U2 U2MODEbits.UEN1
#define _RTSMD_U2 U2MODEbits.RTSMD
#define _IREN_U2 U2MODEbits.IREN
#define _USIDL_U2 U2MODEbits.USIDL
#define _UARTEN_U2 U2MODEbits.UARTEN
#define _PDSEL_U2 U2MODEbits.PDSEL
#define _UEN_U2 U2MODEbits.UEN
// U2STA
#define _URXDA_U2 U2STAbits.URXDA
#define _OERR_U2 U2STAbits.OERR
#define _FERR_U2 U2STAbits.FERR
#define _PERR_U2 U2STAbits.PERR
#define _RIDLE_U2 U2STAbits.RIDLE
#define _ADDEN_U2 U2STAbits.ADDEN
#define _URXISEL0_U2 U2STAbits.URXISEL0
#define _URXISEL1_U2 U2STAbits.URXISEL1
#define _TRMT_U2 U2STAbits.TRMT
#define _UTXBF_U2 U2STAbits.UTXBF
#define _UTXEN_U2 U2STAbits.UTXEN
#define _UTXBRK_U2 U2STAbits.UTXBRK
#define _UTXISEL0_U2 U2STAbits.UTXISEL0
#define _UTXINV_U2 U2STAbits.UTXINV
#define _UTXISEL1_U2 U2STAbits.UTXISEL1
#define _URXISEL_U2 U2STAbits.URXISEL
#define UART2_TX_TRIS TRISFbits.TRISF5
#define UART2_RX_TRIS TRISFbits.TRISF4
#define UART2_TX_LAT LATFbits.LATF5
#define UART2_RX_LAT LATFbits.LATF4
// I/O definitions
#define CR 0x0D // Salto de linea
#define LF 0x0A // Para poner cursor en la siguiente linea
#define BACKSPACE 0x8 // ASCII backspace character code
// useful macros
#define clrscr "\x1b[2J" // Limpiar pantalla
#define home '\x1b[H' // Vuelve a poner el cursor al principio
#define pcr "\r" //

void initRS232_2(void);
void putRS232_2(char);
char getRS232_2( void);
void meterIntEnBuffer(char c);
int getBuffer();
void clear_buffer();