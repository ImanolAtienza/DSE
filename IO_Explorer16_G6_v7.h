
/*
 * Proyecto: DSE_Grupo_6_IO_v7
 * Fichero: IO_Explorer16_G6_v7.h
 *
 * Grupo: G6    Autor: Cristian Mares e Imanol Atienza
 * v7.0	Fecha: 27-1-2016
 */

// Los leds están conectados a tierra ->
// -> Un '1' en la salida del PIC lo enciende
// -> Un '0' lo Apaga

#include "p24hj256gp610A.h"

void Inic_Leds_Explorer16 (void);   //
void Inic_Pulsadores_Explorer16 (void) ; //
void Atender_Joystick(void);

#define TRIS_LED_D3     _TRISA0
#define TRIS_LED_D4     _TRISA1
#define TRIS_LED_D5     _TRISA2
#define TRIS_LED_D6     _TRISA3
#define TRIS_LED_D7     _TRISA4
#define TRIS_LED_D8     _TRISA5
#define TRIS_LED_D9     _TRISA6
#define TRIS_LED_D10    _TRISA7

#define LED_D3      _LATA0
#define LED_D4      _LATA1
#define LED_D5      _LATA2
#define LED_D6      _LATA3
#define LED_D7      _LATA4
#define LED_D8      _LATA5
#define LED_D9      _LATA6
#define LED_D10     _LATA7

#define TRIS_J_fArriba   _TRISC1
#define J_fArriba   _RC1
#define TRIS_J_fDerecha   _TRISC2
#define J_fDerecha   _RC2
#define TRIS_J_fAbajo   _TRISC3
#define J_fAbajo   _RC3
#define TRIS_J_fIzquierda   _TRISC4
#define J_fIzquierda   _RC4

#define TRIS_J_Lateral   _TRISG12
#define J_Lateral   _RG12

#define TRIS_J_Derecho   _TRISG13
#define J_Derecho   _RG13

#define TRIS_J_Gatillo   _TRISD15
#define J_Gatillo   _RD15

#define TRIS_J_Central   _TRISD9
#define J_Central   _RD9

#define TRIS_PULSADOR_S3    TRISDbits.TRISD6
#define PULSADOR_S3         _RD6
#define IE_PULSADOR_S3      _CN15IE

#define TRIS_PULSADOR_S6    TRISDbits.TRISD7
#define PULSADOR_S6         _RD7
#define IE_PULSADOR_S6      _CN16IE

#define TRIS_PULSADOR_S4    TRISDbits.TRISD13
#define PULSADOR_S4         _RD13
#define IE_PULSADOR_S4      _CN19IE

//#define TRIS_PULSADOR_S5    TRISDbits.TRISA7
#define PULSADOR_S5         _RA7
#define IE_PULSADOR_S5      _CN23IE

