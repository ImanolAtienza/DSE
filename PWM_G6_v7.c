
#include "PWM_G6_v7.h"
#include "LCD_Explorer16_24H_G6_v7.h"
#include "Crono_G6_v7.h"

typedef enum{
    CALIBRADO,
    TAKE_MIN,
    TAKE_MAX
} Estado;

typedef struct{
    int n;
    int tMin;
    int tMed;
    int tMax;
    float potToServ_ratio;
    Estado estado;
} Servo;
Servo serv_1;
Servo serv_2;
Servo serv_3;
Servo serv_4;
// Servos por software
Servo serv_5;
Servo serv_6;
Servo serv_7;
Servo serv_8;
Servo serv_9;


int SPWM_state;

int S_OC1RS;
int S_OC2RS;
int S_OC3RS;
int S_OC4RS;
int S_OC5RS;

int start_P = 1;
unsigned char DANCE = 0;
extern char hCmd;
extern unsigned char espera_bailecito;
//extern int dSeg;

// Inicializa Timer2
void Inic_Timer_PWM(int dur){ //dur = duracion en ns
     TMR2 = 0; // Borra el timer
     if (dur!=0){ //Pone a dur la duracion del periodo, a no ser que dur sea 0
         PR6 = PR2 = (((dur/25))/64)-1; // poner dur a 1K para periodo de 40us
     }
     else{
         PR6 = PR2 = 12500-1;
     }

    // T1CON = 0x8000; // enabled, prescaler 1:1, internal clock
    // T1CON = 0b1000000000000000; // enabled, prescaler 1:1, internal clock
     T2CON = 0;
     T6CON = 0;
     // VAlor del prescaler
     T2CONbits.TCKPS = 2;
     T6CONbits.TCKPS =  2;

//    IPC1bits.T2IP = 0x01; // Define prioridad de interrupción del timer
    _TPWMIF = 0; // Borra flag interrupción
    _TPWMIE = 0; // Habilita Interrupción Timer 2

    T2CONbits.TON = 1; // Activa Timer 2
    T6CONbits.TON = 1; // Activa Timer 6
}
void Inic_PWM(void){
    // Inicializacion del Timer2
    Inic_Timer_PWM(0);
    // Inicializa Módulo Output Compare
    serv_1.n=1; // Servo Pinza
    serv_1.tMin=800;
    serv_1.tMed=965;
    serv_1.tMax=1500;
    serv_1.potToServ_ratio=1.0;
    serv_1.estado=CALIBRADO;
    OC1CON = 0;
        OC1CONbits.OCM = 0b000; // Deshabilita módulo Output Compare
        OC1R = serv_1.tMed; // Escribe el duty cycle para el primer pulso PWM
        OC1RS = serv_1.tMed; // Escribe el duty cycle para el segundo pulso PWM
//      OC1CONbits.OCTSEL = 0; // Selecciona Timer 2
        OC1CONbits.OCM = 0b110; // Selecciona el modo PWM sin protección
    serv_2.n=2;
    serv_2.tMin=100;
    serv_2.tMed=360;
    serv_2.tMax=10000;
    serv_2.potToServ_ratio=1.0;
    serv_2.estado=CALIBRADO;
    OC2CON = 0;
        OC2CONbits.OCM = 0b000; // Deshabilita módulo Output Compare
        OC2R = serv_2.tMed; // Escribe el duty cycle para el primer pulso PWM
        OC2RS = serv_2.tMed; // Escribe el duty cycle para el segundo pulso PWM
//      OC2CONbits.OCTSEL = 0; // Selecciona Timer 2
        OC2CONbits.OCM = 0b110; // Selecciona el modo PWM sin protección
    serv_3.n=3;
    serv_3.tMin=100;
    serv_3.tMed=360;
    serv_3.tMax=10000;
    serv_3.potToServ_ratio=1.0;
    serv_3.estado=CALIBRADO;
    OC3CON = 0;
        OC3CONbits.OCM = 0b000; // Deshabilita módulo Output Compare
        OC3R = serv_3.tMed; // Escribe el duty cycle para el primer pulso PWM
        OC3RS = serv_3.tMed; // Escribe el duty cycle para el segundo pulso PWM
//      OC3CONbits.OCTSEL = 0; // Selecciona Timer 2
        OC3CONbits.OCM = 0b110; // Selecciona el modo PWM sin protección
    serv_4.n=4;
    serv_4.tMin=100;
    serv_4.tMed=360;
    serv_4.tMax=10000;
    serv_4.potToServ_ratio=1.0;
    serv_4.estado=CALIBRADO;
    OC4CON = 0;
        OC4CONbits.OCM = 0b000; // Deshabilita módulo Output Compare
        OC4R = serv_4.tMed; // Escribe el duty cycle para el primer pulso PWM
        OC4RS = serv_4.tMed; // Escribe el duty cycle para el segundo pulso PWM
//      OC4CONbits.OCTSEL = 0; // Selecciona Timer 2
        OC4CONbits.OCM = 0b110; // Selecciona el modo PWM sin protección

    // Lata y Trisa de los servos por software
    Lata_SerSoft1 = 0;
    Lata_SerSoft2 = 0;
    Lata_SerSoft3 = 0;
    Lata_SerSoft4 = 0;
    Lata_SerSoft5 = 0;

    Trisa_SerSoft1 = 0;
    Trisa_SerSoft2 = 0;
    Trisa_SerSoft3 = 0;
    Trisa_SerSoft4 = 0;
    Trisa_SerSoft5 = 0;

    serv_5.tMin=300; // Serv 0 brazo
    serv_5.tMax=1500;
    serv_5.tMed =(serv_5.tMax-serv_5.tMin)>>1;
    S_OC1RS =serv_5.tMed + serv_5.tMin;
    serv_5.potToServ_ratio=(serv_5.tMax-serv_5.tMin)/1023.0;

    serv_6.tMin=370; // Serv 1 brazo
    serv_6.tMax=1310;
    S_OC2RS =serv_6.tMed=serv_6.tMax;
    serv_6.potToServ_ratio=(serv_6.tMax-serv_6.tMin)/1023.0;

    serv_7.tMin=510; // Serv 2 brazo
    serv_7.tMax=1510;
    S_OC3RS =serv_7.tMed=serv_7.tMax;
    serv_7.potToServ_ratio=(serv_7.tMax-serv_7.tMin)/1023.0;

    serv_8.tMin=260; // Serv 3 brazo
    serv_8.tMax=1600;
    S_OC4RS =serv_8.tMed=750;
    serv_8.potToServ_ratio=(serv_8.tMax-serv_8.tMin)/1023.0;

    serv_9.tMin=0;
    serv_9.tMax=5000;
   // S_OC5RS =serv_9.tMed=serv_9.tMax;
    S_OC5RS =serv_9.tMed=2500;
    serv_9.potToServ_ratio=(serv_9.tMax-serv_9.tMin)/1023.0;

    SPWM_state = 1;

    _T6IP = 5;
    _T6IF = 0;
    _T6IE = 1;

    set_int_ventana_4Cifras(OC1RS, 6, 10);
}

void calibrar_servo(int s){
    switch(s){
        case 1:
            switch (serv_1.estado){
                case CALIBRADO:
                    serv_1.tMin=100;
                    serv_1.tMax=10000;
                    set_string_ven_serv("T_MIN",0,0);
                    serv_1.estado= TAKE_MIN;
                    break;
                case TAKE_MIN:
                    serv_1.tMin=OC1RS;
                    set_string_ven_serv("T_MAX",0,0);
                    serv_1.estado= TAKE_MAX;
                    break;
                case TAKE_MAX:
                    serv_1.tMax=OC1RS;
                    serv_1.tMed=(serv_1.tMax-serv_1.tMin)/2;
                    serv_1.potToServ_ratio=(serv_1.tMax-serv_1.tMin)/1023.0;
                    set_string_ven_serv("Servo",0,0);
                    serv_1.estado= CALIBRADO;
                    break;
            }
            break;
        case 2:
            switch (serv_2.estado){
                case CALIBRADO:
                    serv_2.tMin=100;
                    serv_2.tMax=10000;
                    set_string_ven_serv("T_MIN",1,0);
                    serv_2.estado= TAKE_MIN;
                    break;
                case TAKE_MIN:
                    serv_2.tMin=OC1RS;
                    set_string_ven_serv("T_MAX",1,0);
                    serv_2.estado= TAKE_MAX;
                    break;
                case TAKE_MAX:
                    serv_2.tMax=OC1RS;
                    serv_2.tMed=(serv_2.tMax-serv_2.tMin)/2;
                    serv_2.potToServ_ratio=(serv_2.tMax-serv_2.tMin)/1023.0;
                    set_string_ven_serv("Servo",1,0);
                    serv_2.estado= CALIBRADO;
                    break;
            }
            break;
        case 3:
            switch (serv_3.estado){
                case CALIBRADO:
                    serv_3.tMin=100;
                    serv_3.tMax=10000;
                    set_string_ven_serv("T_MIN",2,0);
                    serv_3.estado= TAKE_MIN;
                    break;
                case TAKE_MIN:
                    serv_3.tMin=OC1RS;
                    set_string_ven_serv("T_MAX",2,0);
                    serv_3.estado= TAKE_MAX;
                    break;
                case TAKE_MAX:
                    serv_3.tMax=OC1RS;
                    serv_3.tMed=(serv_3.tMax-serv_3.tMin)/2;
                    serv_3.potToServ_ratio=(serv_3.tMax-serv_3.tMin)/1023.0;
                    set_string_ven_serv("Servo",2,0);
                    serv_3.estado= CALIBRADO;
                    break;
            }
            break;
        case 4:
            switch (serv_4.estado){
                case CALIBRADO:
                    serv_4.tMin=100;
                    serv_4.tMax=10000;
                    set_string_ven_serv("T_MIN",3,0);
                    serv_4.estado= TAKE_MIN;
                    break;
                case TAKE_MIN:
                    serv_4.tMin=OC1RS;
                    set_string_ven_serv("T_MAX",3,0);
                    serv_4.estado= TAKE_MAX;
                    break;
                case TAKE_MAX:
                    serv_4.tMax=OC1RS;
                    serv_4.tMed=(serv_4.tMax-serv_4.tMin)/2;
                    serv_4.potToServ_ratio=(serv_4.tMax-serv_4.tMin)/1023.0;
                    set_string_ven_serv("Servo",3,0);
                    serv_4.estado= CALIBRADO;
                    break;
            }
            break;
        case 5:
            switch (serv_5.estado){
                case CALIBRADO:
                    serv_5.tMin=100;
                    serv_5.tMax=10000;
                    set_string_ven_serv("T_MIN",4,0);
                    serv_5.estado= TAKE_MIN;
                    break;
                case TAKE_MIN:
                    serv_5.tMin=S_OC1RS;
                    set_string_ven_serv("T_MAX",4,0);
                    serv_5.estado= TAKE_MAX;
                    break;
                case TAKE_MAX:
                    serv_5.tMax=S_OC1RS;
                    serv_5.tMed=(serv_5.tMax-serv_5.tMin)/2;
                    serv_5.potToServ_ratio=(serv_5.tMax-serv_5.tMin)/1023.0;
                    set_string_ven_serv("Servo",4,0);
                    serv_5.estado= CALIBRADO;
                    break;
            }
            break;
        case 6:
            switch (serv_6.estado){
                case CALIBRADO:
                    serv_6.tMin=100;
                    serv_6.tMax=10000;
                    set_string_ven_serv("T_MIN",5,0);
                    serv_6.estado= TAKE_MIN;
                    break;
                case TAKE_MIN:
                    serv_6.tMin=S_OC1RS;
                    set_string_ven_serv("T_MAX",5,0);
                    serv_6.estado= TAKE_MAX;
                    break;
                case TAKE_MAX:
                    serv_6.tMax=S_OC1RS;
                    serv_6.tMed=(serv_6.tMax-serv_6.tMin)/2;
                    serv_6.potToServ_ratio=(serv_6.tMax-serv_6.tMin)/1023.0;
                    set_string_ven_serv("Servo",5,0);
                    serv_6.estado= CALIBRADO;
                    break;
            }
            break;
        case 7:
            switch (serv_7.estado){
                case CALIBRADO:
                    serv_7.tMin=100;
                    serv_7.tMax=10000;
                    set_string_ven_serv("T_MIN",6,0);
                    serv_7.estado= TAKE_MIN;
                    break;
                case TAKE_MIN:
                    serv_7.tMin=S_OC1RS;
                    set_string_ven_serv("T_MAX",6,0);
                    serv_7.estado= TAKE_MAX;
                    break;
                case TAKE_MAX:
                    serv_7.tMax=S_OC1RS;
                    serv_7.tMed=(serv_7.tMax-serv_7.tMin)/2;
                    serv_7.potToServ_ratio=(serv_7.tMax-serv_7.tMin)/1023.0;
                    set_string_ven_serv("Servo",6,0);
                    serv_7.estado= CALIBRADO;
                    break;
            }
            break;
        case 8:
            switch (serv_8.estado){
                case CALIBRADO:
                    serv_8.tMin=100;
                    serv_8.tMax=10000;
                    set_string_ven_serv("T_MIN",7,0);
                    serv_8.estado= TAKE_MIN;
                    break;
                case TAKE_MIN:
                    serv_8.tMin=S_OC1RS;
                    set_string_ven_serv("T_MAX",7,0);
                    serv_8.estado= TAKE_MAX;
                    break;
                case TAKE_MAX:
                    serv_8.tMax=S_OC1RS;
                    serv_8.tMed=(serv_8.tMax-serv_8.tMin)/2;
                    serv_8.potToServ_ratio=(serv_8.tMax-serv_8.tMin)/1023.0;
                    set_string_ven_serv("Servo",7,0);
                    serv_8.estado= CALIBRADO;
                    break;
            }
            break;
        case 9:
            switch (serv_9.estado){
                case CALIBRADO:
                    serv_9.tMin=100;
                    serv_9.tMax=10000;
                    set_string_ven_serv("T_MIN",8,0);
                    serv_9.estado= TAKE_MIN;
                    break;
                case TAKE_MIN:
                    serv_9.tMin=S_OC1RS;
                    set_string_ven_serv("T_MAX",8,0);
                    serv_9.estado= TAKE_MAX;
                    break;
                case TAKE_MAX:
                    serv_9.tMax=S_OC1RS;
                    serv_9.tMed=(serv_9.tMax-serv_9.tMin)/2;
                    serv_9.potToServ_ratio=(serv_9.tMax-serv_9.tMin)/1023.0;
                    set_string_ven_serv("Servo",8,0);
                    serv_9.estado= CALIBRADO;
                    break;
            }
            break;
    }

}

void center_Servo(int s) {
    switch(s){
        case 1:
            OC1RS = serv_1.tMin+serv_1.tMed;
            break;
        case 2:
            OC2RS = serv_2.tMin+serv_2.tMed;
            break;
        case 3:
            OC3RS = serv_3.tMin+serv_3.tMed;
            break;
        case 4:
            OC4RS = serv_4.tMin+serv_4.tMed;
            break;
        case 5:
            S_OC1RS = serv_5.tMin+serv_5.tMed;
            break;
        case 6:
            S_OC2RS = serv_6.tMin+serv_6.tMed;
            break;
        case 7:
            S_OC3RS = serv_7.tMin+serv_7.tMed;
            break;
        case 8:
            S_OC4RS = serv_8.tMin+serv_8.tMed;
            break;
        case 9:
            S_OC5RS = serv_9.tMin+serv_9.tMed;
            break;
    }
}
void girar_Izq(int s) {
    switch(s){
        case 1:
            if ((OC1RS-10) >= serv_1.tMin) {
                OC1RS -= 10;
            }
            break;
        case 2:
            if ((OC2RS-10) >= serv_2.tMin) {
                OC2RS -= 10;
            }
            break;
        case 3:
            if ((OC3RS-10) >= serv_3.tMin) {
                OC3RS -= 10;
            }
            break;
        case 4:
            if ((OC4RS-10) >= serv_4.tMin) {
                OC4RS -= 10;
            }
            break;
        case 5:
            if ((S_OC1RS-10) >= serv_5.tMin) {
                S_OC1RS -= 10;
            }
            break;
        case 6:
            if ((S_OC2RS-10) >= serv_6.tMin) {
                S_OC2RS -= 10;
            }
            break;
        case 7:
            if ((S_OC3RS-10) >= serv_7.tMin) {
                S_OC3RS -= 10;
            }
            break;
        case 8:
            if ((S_OC4RS-10) >= serv_8.tMin) {
                S_OC4RS -= 10;
            }
            break;
        case 9:
            if ((S_OC5RS-10) >= serv_9.tMin) {
                S_OC5RS -= 10;
            }
            break;
    }
}
void girar_Drch(int s) {
    switch(s){
        case 1:
            if ((OC1RS+10) <= serv_1.tMax) {
                OC1RS +=10;
            }
            break;
        case 2:
            if ((OC2RS+10) <= serv_2.tMax) {
                OC2RS +=10;
            }
            break;
        case 3:
            if ((OC3RS+10) <= serv_3.tMax) {
                OC3RS +=10;
            }
            break;
        case 4:
            if ((OC4RS+10) <= serv_4.tMax) {
                OC4RS +=10;
            }
            break;
        case 5:
            if ((S_OC1RS+10) >= serv_5.tMin) {
                S_OC1RS += 10;
            }
            break;
        case 6:
            if ((S_OC2RS+10) >= serv_6.tMin) {
                S_OC2RS += 10;
            }
            break;
        case 7:
            if ((S_OC3RS+10) >= serv_7.tMin) {
                S_OC3RS += 10;
            }
            break;
        case 8:
            if ((S_OC4RS+10) >= serv_8.tMin) {
                S_OC4RS += 10;
            }
            break;
        case 9:
            if ((S_OC5RS+10) >= serv_9.tMin) {
                S_OC5RS += 10;
            }
            break;
    }
}
void girar(int s,int valor){
    int nv;
    switch(s){
        case 1:
            nv=OC1RS+valor;
            if (nv >= serv_1.tMin && nv <= serv_1.tMax) {
                OC1RS = nv;
            }
            break;
        case 2:
            nv=OC2RS+valor;
            if (nv >= serv_2.tMin && nv <= serv_2.tMax) {
                OC2RS = nv;
            }
            break;
        case 3:
            nv=OC3RS+valor;
            if (nv >= serv_3.tMin && nv <= serv_3.tMax) {
                OC3RS = nv;
            }
            break;
        case 4:
            nv=OC4RS+valor;
            if (nv >= serv_4.tMin && nv <= serv_4.tMax) {
                OC4RS = nv;
            }
            break;
        case 5:
            nv=S_OC1RS+valor;
            if (nv >= serv_5.tMin && nv <= serv_5.tMax) {
                S_OC1RS = nv;
            }
            break;
        case 6:
            nv=S_OC2RS+valor;
            if (nv >= serv_6.tMin && nv <= serv_6.tMax) {
                S_OC2RS = nv;
            }
            break;
        case 7:
            nv=S_OC3RS+valor;
            if (nv >= serv_7.tMin && nv <= serv_7.tMax) {
                S_OC3RS = nv;
            }
            break;
        case 8:
            nv=S_OC4RS+valor;
            if (nv >= serv_8.tMin && nv <= serv_8.tMax) {
                S_OC4RS = nv;
            }
            break;
        case 9:
            nv=S_OC5RS+valor;
            if (nv >= serv_9.tMin && nv <= serv_9.tMax) {
                S_OC5RS = nv;
            }
            break;
    }


}

void set_position(int s, int ingle){
    int angle;

    switch(s){
        case 1:
            angle = (ingle * serv_1.potToServ_ratio)+serv_1.tMin;
            if(angle>serv_1.tMax-5)
                OC1RS=serv_1.tMax-5;
            else if(angle<serv_1.tMin+5)
                OC1RS=serv_1.tMin+5;
            else
                OC1RS=angle;
            break;
        case 2:
            angle = (ingle * serv_2.potToServ_ratio)+serv_2.tMin;
            if(angle>serv_2.tMax-5)
                OC2RS=serv_2.tMax-5;
            else if(angle<serv_2.tMin+5)
                OC2RS=serv_2.tMin+5;
            else
                OC2RS=angle;
            break;
        case 3:
            angle = (ingle * serv_3.potToServ_ratio)+serv_3.tMin;
            if(angle>serv_3.tMax-5)
                OC3RS=serv_3.tMax-5;
            else if(angle<serv_3.tMin+5)
                OC3RS=serv_3.tMin+5;
            else
                OC3RS=angle;
            break;
        case 4:
            angle = (ingle * serv_4.potToServ_ratio)+serv_4.tMin;
            if(angle>serv_4.tMax-5)
                OC4RS=serv_4.tMax-5;
            else if(angle<serv_4.tMin+5)
                OC4RS=serv_4.tMin+5;
            else
                OC4RS=angle;
            break;
        case 5:
            angle = (ingle * serv_5.potToServ_ratio)+serv_5.tMin;
            if(angle>serv_5.tMax-5)
                S_OC1RS=serv_5.tMax-5;
            else if(angle<serv_5.tMin+5)
                S_OC1RS=serv_5.tMin+5;
            else
                S_OC1RS=angle;
            break;
        case 6:
            angle = (ingle * serv_6.potToServ_ratio)+serv_6.tMin;
            if(angle>serv_6.tMax-5)
                S_OC2RS=serv_6.tMax-5;
            else if(angle<serv_6.tMin+5)
                S_OC2RS=serv_6.tMin+5;
            else
                S_OC2RS=angle;
            break;
        case 7:
            angle = (ingle * serv_7.potToServ_ratio)+serv_7.tMin;
            if(angle>serv_7.tMax-5)
                S_OC3RS=serv_7.tMax-5;
            else if(angle<serv_7.tMin+5)
                S_OC3RS=serv_7.tMin+5;
            else
                S_OC3RS=angle;
            break;
        case 8:
            angle = (ingle * serv_8.potToServ_ratio)+serv_8.tMin;
            if(angle>serv_8.tMax-5)
                S_OC4RS=serv_8.tMax-5;
            else if(angle<serv_8.tMin+5)
                S_OC4RS=serv_8.tMin+5;
            else
                S_OC4RS=angle;
            break;
        case 9:
            angle = (ingle * serv_9.potToServ_ratio)+serv_9.tMin;
            if(angle>serv_9.tMax-5)
                S_OC5RS=serv_9.tMax-5;
            else if(angle<serv_9.tMin+5)
                S_OC5RS=serv_9.tMin+5;
            else
                S_OC5RS=angle;
            break;
    }
}

void reset_servs(){
        OC1RS=serv_1.tMed;
        OC2RS=serv_2.tMed;
        OC3RS=serv_3.tMed;
        OC4RS=serv_4.tMed;
        S_OC1RS=serv_5.tMed;
        S_OC2RS=serv_6.tMed;
        S_OC3RS=serv_7.tMed;
        S_OC4RS=serv_8.tMed;
        S_OC5RS=serv_9.tMed;
}

void un_movimiento_sexy(void) {
    /*
     * S1
     *  min 750
     *  max 1170
     * S2
     *  min1 1025
     *  min2 1025
     *  max1 1245
     *  max2 1245
     * S3
     *  min1 1485
     *  min2 1036
     *  max1 688
     *  max2 1036
     * S4
     *  min 652
     *  max 1320
     * H1
     *  min 843
     *  max 1430
     */
    reset_servs();
    while(espera_bailecito==0){}espera_bailecito=0;
    DANCE=1;
    while(DANCE){
        if(hCmd=='M'){
            break;
            hCmd=0;
        }
        S_OC1RS=750;
        S_OC2RS=1025;
        S_OC3RS=1036;
        S_OC4RS=652;
        OC1RS=843;
        //espera
        while(espera_bailecito==0){}espera_bailecito=0;
        
        S_OC1RS=1170;
        S_OC2RS=1025;
        S_OC3RS=688;
        S_OC4RS=1320;
        OC1RS=1430;
        //espera
        while(espera_bailecito==0){}espera_bailecito=0;

        S_OC1RS=750;
        S_OC2RS=1245;
        S_OC3RS=1036;
        S_OC4RS=652;
        OC1RS=843;
        //espera
        while(espera_bailecito==0){}espera_bailecito=0;

        S_OC1RS=1170;
        S_OC2RS=1245;
        S_OC3RS=1485;
        S_OC4RS=1320;
        OC1RS=1430;
        //espera
        while(espera_bailecito==0){}espera_bailecito=0;
    }
    reset_servs();
}

// Servos por Software

//INTERRUPCION CONTROL SERVOS

void _ISR _T6Interrupt(void){
//Rebose del timer

    switch (SPWM_state)
    {
        case 1 :	//Cargamos valor  servo 1
            Lata_SerSoft1 = 1; //activamos señal
            TMR6 = 0;
            PR6 = S_OC1RS;
            SPWM_state = 2;
            break;
        case 2 :	//Cargamos valor 2º servo
            Lata_SerSoft1 = 0; //desactivamos señal
            Lata_SerSoft2 = 1; //activamos señal
            TMR6 = 0;
            PR6 = S_OC2RS;
            SPWM_state = 3;
            break;
        case 3 :	//Cargamos valor primer servo
            Lata_SerSoft2 = 0; //desactivamos señal
            Lata_SerSoft3 = 1; //activamos señal
            TMR6 = 0;
            PR6 = S_OC3RS;
            SPWM_state = 4;
            break;

        case 4 :	//Cargamos valor primer servo
            Lata_SerSoft3 = 0; //desactivamos señal
            Lata_SerSoft4 = 1; //activamos señal
            TMR6 = 0;
            PR6 = S_OC4RS;
            SPWM_state = 5;
            break;
        case 5 :	//Cargamos valor primer servo
            Lata_SerSoft4 = 0; //desactivamos señal
            Lata_SerSoft5 = 1; //activamos señal
            TMR6 = 0;
            PR6 = S_OC5RS;
            SPWM_state = 6;
//	    break;
        case 6 :	//Cargamos valor primer servo
            Lata_SerSoft5 = 0; //desactivamos señal
            Lata_SerSoft4 = 0; //desactivamos señal
            Lata_SerSoft3 = 0; //desactivamos señal
            Lata_SerSoft2 = 0; //desactivamos señal
            Lata_SerSoft1 = 0; //desactivamos señal

            TMR6 = 0;
             PR6 = 12500 - (S_OC1RS +S_OC2RS +S_OC3RS +S_OC4RS +S_OC5RS);
            SPWM_state = 1;
            break;
    }

    _T6IF = 0;
} //FIN de _ISR _T5Interrupt
