/*
Proyecto: DSE_Grupo_6_Timer_v7
Fichero: Crono_G6_v7.c

Grupo: G6    Autor: Imanol Atienza y Christian Mares
v7.0	Fecha: 15-2-2016
// Explicar que realiza
 Descripción
 *
 */

#include "Crono_G6_v7.h"
#include "IO_Explorer16_G6_v7.h"
#include "LCD_Explorer16_24H_G6_v7.h"

unsigned int mSeg ; // Milisegundos
unsigned int cSeg ; // Centesimas de segundo
unsigned int dSeg ; // Decimas de segundo
unsigned int uSeg ; // Unidades de Segundo
unsigned int DSeg ; // Decenas de Segundo
unsigned int uMin ; // Unidades de Minuto
unsigned int DMin ; // Decenas de Minuto
unsigned int Corriendo = 1;

void Inic_Timer_1 (void){
 mSeg = 0;
 cSeg = 0;
 dSeg = 0;
 uSeg  = 0;
 DSeg  = 0;
 uMin  = 0;
 DMin  = 0;
// Init Timer
 TMR1 = 0; // Borra el timer
 //PR1 = 40000-1; // Configura el periodo -> Mhz x 1000ms - 1
 PR1 = 40000-1; //

// T1CON = 0x8000; // enabled, prescaler 1:1, internal clock
// T1CON = 0b1000000000000000; // enabled, prescaler 1:1, internal clock
 T1CON = 0;
 T1CONbits.TCKPS = 0; // VAlor del prescaler
 //_T1CONbits.TON; // Pone en marcha el Timer 1
// _T1IP = 4; // prioridad por defecto
 _T1IP = 6; // Equivale a _T1IP
 _T1IF = 0; // Borra flag de interrupción
 //IFS0bits.T1IF = 0; // Equivale a _T1IF
 _T1IE = 1; // Habilita interrupción del Timer 1
 // IEC0bits.T1IE = 1; // Equivale a _T1IE
T1CONbits.TON =1; // Pone en marcha el Timer 1
}

void ini_Crono(void){
    Corriendo = 1;
}
void pause_Crono(void){
    Corriendo = 0;
}
void stop_Crono(void){
    Corriendo = 0;
    LED_D10 = 0;
    LED_D9 = 0;
    LED_D8 = 0;
    mSeg = 0;
    cSeg = 0;
    dSeg = 0;
    uSeg = 0;
    DSeg = 0;
    uMin = 0;
    DMin = 0;
}

void get_mseg(unsigned char *mseg){
    mseg[0] = (char)(mSeg)+48;// +48 porque  el '0' tiene  el valor 48 en ASCII
    mseg[1] = (char)(cSeg)+48;// Los  enteros  en ASCII van  del 48(0) al 58(9)
    mseg[2] = (char)(dSeg)+48;// Separando  las  unidades,  decenas,  centenas,
                                 // etc. se puede convertir a ASCII en un periquete.
}
void get_seg(unsigned char * seg){
    seg[0] = (char)(uSeg)+48;
    seg[1] = (char)(DSeg)+48;
}
void get_min(unsigned char * min){
    min[0] = (char)(uMin)+48;
    min[1] = (char)(DMin)+48;
}

unsigned int get_mSeg(){
    return mSeg;
}
unsigned int get_cSeg(){
    return cSeg;
}
unsigned int get_dSeg(){
    return dSeg;
}
unsigned int get_uSeg(){
    return uSeg;
}
unsigned int get_DSeg(){
    return DSeg;
}
unsigned int get_uMin(){
    return uMin;
}
unsigned int get_DMin(){
    return DMin;
}

void update_crono_line(){
    set_char_linea_2( mSeg+48,14);
    set_char_linea_2( cSeg+48,13);
    set_char_linea_2( dSeg+48,12);
    set_char_linea_2( uSeg+48,10);
    set_char_linea_2( DSeg+48,9);
    set_char_linea_2( uMin+48,7);
    set_char_linea_2( DMin+48,6);
}
unsigned char main_itr, clear_screen,listen_joystick_buttons,espera_bailecito;
void _ISR _T1Interrupt(void){

    if(Corriendo)
    {

        mSeg++;
        if(mSeg > 9){
            mSeg = 0;
            cSeg++;
            main_itr=1;
            listen_joystick_buttons=1;
        }
        if(cSeg > 9){
            LED_D10 = !LED_D10;
            cSeg = 0;
            dSeg++;
        }
        if(dSeg>9){
            LED_D9 = !LED_D9;
            dSeg = 0;
            uSeg++;
            espera_bailecito=1;
        }
         if(uSeg>9){
            uSeg = 0;
            DSeg ++;
            clear_screen=1;
        }
        if(DSeg>5){
            LED_D8 = !LED_D8;
            DSeg = 0;
            uMin ++;
        }
        if(uMin>9){
            uMin = 0;
            DMin ++;
        }
        if(DMin>5){
            DMin=0;
        }
    }

    _T1IF = 0;
}
