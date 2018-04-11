/*
 * Proyecto: DSE_Grupo_6_IO_v7
 * Fichero: CAD_G6_v7.c
 *
 * Grupo: G6    Autor: Cristian Mares e Imanol Atienza
 * v7.0	Fecha: 27-1-2016
 */

#include "p24hj256gp610A.h"
#include "LCD_Explorer16_24H_G6_v7.h"
#include "CAD_LAVB_G6_v7.h"
#include "PWM_G6_v7.h"

//int estadoCAD=5;
//int ADCvalores[16];
int estCalibrar=0;
int potencLV=0,potencNV, // NV = New Value y LV = Last Value
    joy_X_LV=0,joy_X_NV,
    joy_Y_LV=0,joy_Y_NV,
    joy_Z_LV=0,joy_Z_NV;

unsigned char potencCF, // Potenciometro Change Flag
              joy_X_CF, // Joystick X      "      "
              joy_Y_CF, // Joystick Y      "      "
              joy_Z_CF; // Joystick Z      "      "

void switch_estCalibrar(void){
    estCalibrar=!estCalibrar;
    _CH0SA=8;
}

//CONFIGURAMOS Timer 4 como activador de la conversión
void Inic_Timer_CAD (int dur)
{
     TMR4 = 0; // Borra el timer
     if (dur!=0){ //Pone a dur la duracion del periodo, a no ser que dur sea 0
         PR4 = (dur/25)-1; // poner dur a 1K para periodo de 40us
     }
     else{
         PR4 = 40000-1; // interrumpe cada 1ms (igual a: 1.000.000/25)
     }

     T1CON = 0x8000; // enabled, prescaler 1:1, internal clock
    // T1CON = 0b1000000000000000; // enabled, prescaler 1:1, internal clock
     T4CON = 0;

    _T4IF = 0; // Borra flag interrupción
    _T4IE = 1; // Habilita Interrupción Timer 2
    T4CONbits.TON = 1; // Activa Timer
}

//CONFIGURAMOS el ADC1 PARA MUESTR EAR AN0, AN1, ?., AN9 por el CH0
void Inic_CAD (void) {
Inic_Timer_CAD(0);
AD1CON1= 0 ;
//_ADSIDL = __; // 1= Se para en modo Idle / 0=Continua en modo Idle
//_ADDMABM = __; // 1= búfer DMA en orden de conversión / 0= modo Scatter/Gather
//_AD12B = __; // 1= Modo operacion de 12 bits / 0=Modo operacion de 10 bits
//_FORM = __ ; // Formato datos salida: 00= Integer (0000 00dd dddd dddd)
 // 01= Signed integer (ssss sssd dddd dddd)
_SSRC = 0b111; // Termina muestreo y comienza conversion
 // 111=Auto-coonvert / 010=TMR3 ADC1 y TMR5 ADC2
 // 001=INT0 / 000= hay que borrar SAMP
//_SIMSAM = __; // 1= Muestreo simultaneo / 0=Muestreo canales secuencialmente
//_ASAM = __; // 1= Muestreo comienza después de la última conversión
 // 0= Muestreo comienza cuando SAMP ? 1
//_SAMP= __; // Si ASAM=0; SAMP?1 inicia muestreo.
//_ADON = 1; // Comienzo de conversión
//_DONE = __; // Si SRRC=000; SAMP?0 finaliza muestreo y inicia conversión
AD1CON2= 0 ;
//_VCFG = __; // Vref+- 000=ACdd y AVss / 001=Vref+ y AVss
 // 010=AVdd y Vref- / 011=Vref+ y Vref-
//_CSCNA = __; // 1= Escaneo canal cero / 0= no escaneo
//_CHPS = __; // Canales utilizados: 00= CH0 / 01=CH0, CH1 / 1x= CH0, 1, 2 y CH3
//_BUFM = __; // 1= 2 medios Buffers / 0= 1 buffer
//_SMPI = __; // Incrementos dir DMA +1
//_BUFS solo de lectura: con BUFM=1-> 1=activa 2º mitad buffer / 0= activa 1ª mitad
//_ALTS = __; // Modo de muestreo: 1= Alternado / 0= Siempre canal A

AD1CON3 = 0;
_ADRC = 0; // 1= reloj RC / 0= Reloj CPU
_SAMC = 31; // Tiempo auto muestreo = nºTad
_ADCS = 3; // TAD >75ns = Tcy(ADCS+1) <--- ATENCION!!! extryendo ADCS+1 concluimos que: 3+1=4 OK?

AD1CON4 = 0;
//_DMABL = __; // Cada buffer contiene 8 palabras
AD1CHS123 = 0; // selección entrada canal 1,2 y 3
//_CH123NA = __; // Selección entrada negativa Canales 1, 2, y 3 pata Sample A
//_CH123NB = __; // 0x? CH1=CH2=CH3-> Vref- / 10? CH1=AN6, CH2=AN7 CH3=AN8
 // 11? CH1=AN9, CH2=AN10, CH3=AN11
//_CH123SA = __; // Selección entrada positiva Canales 1, 2, y 3 para Sample A
//_CH123SB = __; // 1= AN3-AN4-AN5 / 0= AN0-AN1-AN2
AD1CHS0 = 0; // selección de entrada canal 0
//_CH0NA = __; // Entrada negativa 1= AN1 / 0 = Vref-
//_CH0NB = __;
_CH0SA = 5; // Entrada positiva = canal xx (CH0SA)
//_CH0SB = __;
AD1CSSH = 0; // Selección entradas escaneo 16 a 31 / 1= seleccionada /0= NO
AD1CSSL = 0; // Selección entradas escaneo 0 a 15.
AD1PCFGH = 0xFFFF; // Configura entradas: 1= digital / 0= Analog
AD1PCFGL = 0xFFFF; // Analógicas AN0 a AN9
//_PCFG0= // AN0
_PCFG4= 0; // AN -> RB -> Sensor Temperatura
_PCFG5= 0; // AN -> RB -> Potenciómetro
_PCFG0= 0; // AN -> RB -> Joystick_Z
_PCFG8= 0; // AN -> RB -> Joystick_X
_PCFG9= 0; // AN -> RB -> Joystick_Y
_ADON = 1;
//IFS0: Flags de interrupciones, ver pag. 30 apuntes arquitectura
_AD1IF = 0; // Flag interrupción del ADC1
//IEC0: Habilitaciones
_AD1IE = 1; // Habilitación de la interrupción
//IPC3: Prioridades
_AD1IP = 2;

//estadoCAD=2;

AD1CON1bits.SAMP = 1;
} // Fin Inic_CAD

void _ISR _ADC1Interrupt(void)
{
    //ADCvalores[_CH0SA-1] = ADC1BUF0;     // Leer valor de conversión anterior
    if(!estCalibrar){
        add_new(_CH0SA,ADC1BUF0);

        // Seleccionar entrada nueva conversión
        if(_CH0SA == 4) // Si es el S.Temperatura
            _CH0SA = 5;
        else if(_CH0SA == 5){ // Si es el S.Potenciometro
            potencNV = get_media(5) & 0xfff8; // Coge Val Medio de la SA en cuestion (Potenciometro), pone a 0 los tres bits de menos peso y guardamos el valor
            potencCF = potencLV != potencNV; // Levanta el Change Flag si el nuevo valor es distinto al ultimo
            potencLV = potencNV; // Guarda nuevo valor en el ultimo
            _CH0SA = 8; // Cambiamos de pata de conversion analogica
        }
        else if(_CH0SA == 8){ // Si es el S.Joystick X
            joy_X_NV = get_media(8) & 0xfff8;
            joy_X_CF = joy_X_LV != joy_X_NV;
            joy_X_LV = joy_X_NV;
            _CH0SA = 9;
        }
        else if(_CH0SA == 9){ // Si es el S.Joystick Y
            joy_Y_NV = get_media(9) & 0xfff8;
            joy_Y_CF = joy_Y_LV != joy_Y_NV;
            joy_Y_LV = joy_Y_NV;
            _CH0SA = 0;
        }
        else if(_CH0SA == 0){ // Si es el S.Joystick Z
            joy_Z_NV = get_media(0) & 0xfff8;
            joy_Z_CF = joy_Z_LV != joy_Z_NV;
            joy_Z_LV = joy_Z_NV;
            _CH0SA = 4;
        }
    }
    else{
        calibrar(_CH0SA,ADC1BUF0);

        set_int_ventana_4Cifras(ADC1BUF0, 4, 10);
        set_int_ventana_4Cifras(ADC1BUF0, 5, 10);

        if(_CH0SA == 8){ // Si es el S.Joystick X
            joy_X_NV = get_media(8) & 0xfff8;
            joy_X_CF = joy_X_LV != joy_X_NV;
            joy_X_LV = joy_X_NV;
            _CH0SA = 9;
        }
        else if(_CH0SA == 9){ // Si es el S.Joystick Y
            joy_Y_NV = get_media(9) & 0xfff8;
            joy_Y_CF = joy_Y_LV != joy_Y_NV;
            joy_Y_LV = joy_Y_NV;
            _CH0SA = 8;
        }
    }

    _AD1IF = 0;            // Flag de interrupción
}

void _ISR _T4Interrupt(){
    AD1CON1bits.SAMP = 1; // Lanza la conversión
    _T4IF=0;
}