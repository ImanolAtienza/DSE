/*
 * Proyecto: DSE_Grupo_6_IO_v7
 * Fichero: IO_Explorer16_G6_v7.c
 *
 * Grupo: G6    Autor: Cristian Mares e Imanol Atienza
 * v7.0	Fecha: 27-1-2016
 */

#include "IO_Explorer16_G6_v7.h"
#include "Crono_G6_v7.h"
#include "PWM_G6_v7.h"
#include "CAD_LAVB_G6_v7.h"
#include "CAD_G6_v7.h"

extern float sensibilidad;
extern int potencLV,potencNV,
           joy_X_LV,joy_X_NV,cenVX,
           joy_Y_LV,joy_Y_NV,cenVY,
           joy_Z_LV,joy_Z_NV;
extern unsigned char potencCF,
                      joy_X_CF,
                      joy_Y_CF,
                      joy_Z_CF;

// ================= LEDs ===========================
// Inicializa LEDS placa Explorer16: RA0, ..., RA7
void Inic_Leds_Explorer16 (void)
// Inicializa las patas de los leds de salida y el valor inicial
{
// OJO -> faltan algunos

    TRIS_LED_D3= 0;    // Led_3,  '0' -> Salida (Output) / '1' -> Entrada (Input)
    TRIS_LED_D4= 0;
    TRIS_LED_D5= 0;
    TRIS_LED_D6= 0;
    TRIS_LED_D7= 0;
    TRIS_LED_D8= 0;
    TRIS_LED_D9= 0;
    TRIS_LED_D10=0;

// Los leds están conectados a tierra ->
// -> Un '1' en la salida del PIC lo enciende
// -> Un '0' lo Apaga
    LED_D3= 0;  // Valor de inicio LED_3 -> Apagado
    LED_D4= 0;
    LED_D5= 0;
    LED_D6= 0;
    LED_D7= 0;
    LED_D8= 0;
    LED_D9= 0;
    LED_D10=0;

} // Fin Inic_Leds


// =================== Pulsadores =========================
// Inicializa PULSADORES placa Explorer16
// S5: RA7 ojo coincide con led 10
// S3= RD6, S4= RD13, S6= RD7
// Están en baja, conectados a tierra -> Pulsado= 0

unsigned int Pulsado_S3;

void Inic_Pulsadores_Explorer16 (void)
{
//  TRISDbits.TRISD6 = 1;	//1= IN (entrada) 0= Out (salida)
    TRIS_PULSADOR_S3 = 1;       //1= IN (entrada) 0= Out (salida)
    TRIS_PULSADOR_S4 = 1;
    TRIS_PULSADOR_S6 = 1;

    _TRISC1 = 1;
    _TRISC2 = 1;
    _TRISC3 = 1;
    _TRISC4 = 1;

    _TRISG12 = 1;
    _TRISG13 = 1;
    _TRISD14 = 1;
    _TRISD9 = 1;



//  _TRISA7= 1;	// OJO -> Pata compartida con LED_10

//    _CN15IE= 1;	// Habilita interrupción asociada al Pulsador S3 (RD6)
     IE_PULSADOR_S3 = 1;     //Habilita interrupción
     IE_PULSADOR_S4 = 1;
     IE_PULSADOR_S5 = 1;
     IE_PULSADOR_S6 = 1;

// _CNxxE= 1;	// Habilita interrupción asociada al Pulsador S6 (RD7)

    _CNIP = 2;  //Prioridad baja
    _CNIF = 0;	//Borra el Flag común de todas la CN
                // P4HJ256GP610A.h -> IFS1 -> #define _CNIF IFS1bits.CNIF
    _CNIE = 1;  // Habilita interrupción por CN
                //P4HJ256GP610A.h -> IEC1 -> #define _CNIE IEC1bits.CNIE
}   //FIN Inic_Pulsadores_Explorer16

void Atender_Joystick(void) {
//    if(J_fArriba==0)display_upper_line();
//    if(J_fAbajo==0)display_lower_line();
//    if(J_fDerecha==0)display_lower_line();
//    if(J_fIzquierda==0)display_lower_line();

    if(_RD9==0){
        Nop();
        girar(5,(int)((joy_X_LV-cenVX)*sensibilidad));
        Nop();
    }
    if (_RD14==0 && J_Lateral==0){
        girar(6,(int)((joy_Y_LV-cenVY)* sensibilidad));
        girar(7,(int)((joy_Y_LV-cenVY)* sensibilidad));
    }
    else if (J_Derecho==0 && J_Lateral==0){
            girar(7,((int)((joy_Y_LV-cenVY)* sensibilidad))*-1);
            girar(8,((int)((joy_Y_LV-cenVY)* sensibilidad))*-1);
    }
    else{
        if (_RD14==0){
            girar(6,(int)((joy_Y_LV-cenVY)* sensibilidad));
        }
        if(J_Derecho==0) {
            girar(7,((int)((joy_Y_LV-cenVY)* sensibilidad))*-1);
        }
        if(J_Lateral==0){
            girar(8,(int)((joy_Y_LV-cenVY)* sensibilidad));
        }
    }

    if(joy_Z_CF)
        set_position(1,get_media(0));
}

void _ISR _CNInterrupt(void)
{
    if(!PULSADOR_S3){
        ini_Crono();
    }
    else if(!PULSADOR_S4){
        pause_Crono();
    }
    else if(!PULSADOR_S6){
        stop_Crono();
    }

    _CNIF = 0;
}

