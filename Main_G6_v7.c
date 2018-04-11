/*
 * Proyecto: DSE_Grupo_6_IO_v7
 * Fichero: Main_IO_G6_v7.c
 *
 * Grupo: G6    Autor: Cristian Mares e Imanol Atienza
 * v7.0	Fecha: 27-1-2016
 */
// Con el oscilador va 10 veces más rapido
#include "Main_G6_v7.h"
#include "p24hj256gp610A.h"
#include "IO_Explorer16_G6_v7.h"
#include "LCD_Explorer16_24H_G6_v7.h"
#include "Crono_G6_v7.h"
#include "Oscilador_G6_v7.h"
#include "UART_G6_v7.h"
#include "CAD_G6_v7.h"
#include "CAD_LAVB_G6_v7.h"
#include "PWM_G6_v7.h"
#include "i2c_Func_DSE.h"
#include "SRF08_G6_v7.h"
#include <math.h>


//const unsigned char texto []= "                "
//                              "                ";
//const unsigned char texto []= "-- PIC24 --  G6 "//Para la ventana original. Solo tiene 2 lineas.
//                              "Crono --/--/--- ";
#if(!CALCULATE_CPU)
    const unsigned char texto []= "-- PIC24 --  G6 "//Para la ventana apliada.
                              "Crono --/--/--- "    //2
                              "Potenc.  -    - "    //3
                              "Termom.  -    - "    //4
                              "JoystX   -    - "    //5
                              "JoystY   -    - "    //6
                              "JoystZ   -    - "    //7
                              "Servo x  -    - "    //8
                              "Sensor.  -    - ";   //9
#else
    const unsigned char texto []= "-- PIC24 --  G6 "//Para la ventana apliada.
                              "Crono --/--/--- "    //2
                              "Potenc.  -    - "    //3
                              "Termom.  -    - "    //4
                              "JoystX   -    - "    //5
                              "JoystY   -    - "    //6
                              "JoystZ   -    - "    //7
                              "Servo x  -    - "    //8
                              "Sensor.  -    - "    //9
                              "free CPU     %  "    //1'cpu
                              "max f CPU    %  ";   //2'cpu
#endif

_FOSCSEL (FNOSC_PRI); // Primary oscillator (XT, HS, EC) w/ PPL
_FOSC (FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);
 // Clock switching and clock monitor: Both
 // disable, OSC2 is clock O/P,XT oscillator
_FWDT (FWDTEN_OFF); // Watchdog Timer:Disabled

extern int potencLV,potencNV,
           joy_X_LV,joy_X_NV,cenVX,
           joy_Y_LV,joy_Y_NV,cenVY,
           joy_Z_LV,joy_Z_NV;
extern unsigned char potencCF,
                      joy_X_CF,
                      joy_Y_CF,
                      joy_Z_CF;
extern unsigned char main_itr, listen_joystick_buttons;
extern unsigned char clear_screen;

extern int display_in_serv_line;

float sensibilidad = 0;
unsigned int CPUrest = 0, maxCPUrest = 99;
void le_devo_una_cancion_y_algunos_besos_que_valen_mas_que_el_oro_del_Peru(){
    /*
     * Espera el tiempo que quede hasta cumplir la centesima de segundo y
     * calcula el uso de cpu.
     */
        while(main_itr == 0){
            CPUrest++;
        }

        CPUrest=CPUrest*0.0017500262; // El valor decimal corresponde a 100 / 57.142
        if(CPUrest<maxCPUrest){
            maxCPUrest=CPUrest;
        }
        set_int_ventana_2Cifras(CPUrest,9,10);
        set_int_ventana_2Cifras(maxCPUrest,10,10);
       CPUrest = 0;
       main_itr=0;
}

char hCmd = 0;
void enviar_comando(char c){
    if (c>='0' && c<='9')
        meterIntEnBuffer(c);
    else if(c == '\n'){
        set_position(display_in_serv_line+1,getBuffer());
        clear_buffer();
    }
//    else if(c == 'e')
//        stop_Crono();
//    else if(c == 'r')
//        ini_Crono();
//    else if(c == 't')
//        pause_Crono();
    else if(c == 'r'){
        reset_servs();
    }
    else if(c == 'q'){
        display_upper_line();
    }
    else if(c == 'a'){
        display_lower_line();
    }
    else if(c == 's'){
        display_prev_servo();
    }
    else if(c == 'w'){
        display_next_servo();
    }
    else if(c == 'z'){
        clear_screen=1;
    }
    else if(c == 'c'){
        switch_estCalibrar();
    }
    else if(c == 'v'){
        Nop();
        calibrar_servo(display_in_serv_line+1);
    }
    else if(c == 'j'){
        girar_Izq(display_in_serv_line+1);
    }
    else if(c == 'k'){
        center_Servo(display_in_serv_line+1);
    }
    else if(c == 'l'){
        girar_Drch(display_in_serv_line+1);
    }
    else if(c == 'm') {
        un_movimiento_sexy();
    }

    hCmd = 0;
}

int main (void)
{
Nop();
Nop();
Inic_Oscilador();	//Seleciona Frecuencia del oscilador principal

    AD1PCFGL= 0xFFFF;   // Pone todas las patas analógicas de I/O digital
    AD1PCFGH= 0xFFFF;   // Por defecto las patas analógicas están habilitadas

    // ========================
// Inicializaciones
Nop();
Nop();

Inic_Leds_Explorer16();
Inic_Pulsadores_Explorer16();
Inic_Timer_1();
initRS232_2();
Inic_CAD();

////////===========================================================================================//////////
Inic_LCD(); //Inicializar LCD                                                                             //
copiar_FLASH_RAM(texto); //Terminar inicialiazación                                                      //
_TLCDIE = 1;// Habilitar interrupciones del timer del LCD(TIMER2) para comenzar el funcionamiento normal//
///////==============================================================================================////
Inic_PWM();
InitI2C_1();
Inic_SFR08();



//change_dirI2C(0xE4, 0xE2);
// Bucle Principal
    while(1)
    {
//    Nop();
//    Nop();

        if(listen_joystick_buttons){
            Atender_Joystick();
            listen_joystick_buttons=0;
        }

        update_crono_line();

        //Actualizar ventana con valores obtenidos por CAD
        if(potencCF) {
            set_int_ventana_4Cifras(potencLV,2, 10);
            sensibilidad = potencLV/20460.0f;
        }
        set_int_ventana_4Cifras(get_termom_media(), 3, 10);
        if(joy_X_CF)
            set_int_ventana_4Cifras(joy_X_LV,4, 10);
        if(joy_Y_CF)
            set_int_ventana_4Cifras(joy_Y_LV,5, 10);
        if(joy_Z_CF)
            set_int_ventana_4Cifras(joy_Z_LV,6, 10);

        //Actualizar linea de servos
        update_serv_line();

//        set_int_ventana_4Cifras((int)asinf(0.25f),8,10);

        if(hCmd) {
            enviar_comando(hCmd);
        }

    if(CALCULATE_CPU)
        le_devo_una_cancion_y_algunos_besos_que_valen_mas_que_el_oro_del_Peru();
    } // Fin while(1)

} //Fin main
