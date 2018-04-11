/*
 * Proyecto: DSE_Grupo_6_IO_v7
 * Fichero: LCD_Explorer16_24H_G6_v7.c
 *
 * Grupo: G6    Autor: Cristian Mares e Imanol Atienza
 * v7.0	Fecha: 27-1-2016
 */

#include "LCD_Explorer16_24H_G6_v7.h"
#include "Crono_G6_v7.h"
#include "Main_G6_v7.h"

typedef enum{
    CLEAR_TT,              //Estado para vaciar Tera Term
    RENDER,
    SEL_LINE_1,          //Estados para comportamiento
    SEL_LINE_2,         //el en la rutina de atencion
    WR_LINE_1,         //al TIMER2
    WR_LINE_2
} Estado;
Estado render = SEL_LINE_1; //Variables de estado

int f=0,y=0;                                //Variables para rutina de

//PARA BORRAR//unsigned char min[2], seg[2], mseg[3]; //atencion a TIMER2

//unsigned char ventana_LCD [2][16]; // Reserva 2x16 bytes //Ventana vieja
unsigned char ventana_LCD [LINES][16]; // Reserva 4x16 bytes //Ventana ampliada
unsigned char ventana_servos [9][16]={"Servo H1 -    - ",//0
                                      "Servo H2 -    - ",//1
                                      "Servo H3 -    - ",//2
                                      "Servo H4 -    - ",//3
                                      "Servo S1 -    - ",//4
                                      "Servo S2 -    - ",//5
                                      "Servo S3 -    - ",//6
                                      "Servo S4 -    - ",//7
                                      "Servo S5 -    - ",};//8
extern int S_OC1RS;
extern int S_OC2RS;
extern int S_OC3RS;
extern int S_OC4RS;
extern int S_OC5RS;

int display_in_sec_line=0;//Solo se vive una vez. Ademas, esta variable solo deberia adoptar valores entre 0 y LINEAS
int display_in_serv_line=4;//Solo se vive una vez. Ademas, esta variable solo deberia adoptar valores entre 0 y LINEAS


//====================== OPERACIONES DE INICIALIZACION =========================
void copiar_FLASH_RAM (const unsigned char *texto_LCD_1){
   // Pasa un texto de flash a RAM
    int i,j;
    for(i=0; i<LINES; i++){//Ajustar a la ventana que se este usando
        for(j=0; j<16; j++){
            ventana_LCD [i][j] = texto_LCD_1 [(i*16)+j];

        }
    }
}

void Inic_Timer_LCD(unsigned int dur){//dur = duracion en ns
// Init Timer
 TMR3 = 0; // Borra el timer
 //PR1 = 40000-1; // Configura el periodo -> Mhz x 1000ms - 1
 if (dur!=0){ //Pone a dur la duracion del periodo, a no ser que dur sea 0
     PR3 = (dur/25)-1; // poner dur a 1K para periodo de 40us
 }
 else{
     PR3 = 40000-1; // interrumpe cada 1ms (igual a: 1.000.000/25)
 }

// T1CON = 0x8000; // enabled, prescaler 1:1, internal clock
// T1CON = 0b1000000000000000; // enabled, prescaler 1:1, internal clock
 T3CON = 0;
 T3CONbits.TCKPS = 0; // VAlor del prescaler
 //_T1CONbits.TON; // Pone en marcha el Timer 1
// _T1IP = 4; // prioridad por defecto
// IPC0bits.T1IP = 4; // Equivale a _T1IP
 _T3IP = 3;
 _TLCDIF = 0; // Borra flag de interrupción
 //IFS0bits.T1IF = 0; // Equivale a _T1IF
 _TLCDIE = 0; // Habilita interrupción del Timer 1
 // IEC0bits.T1IE = 1; // Equivale a _T1IE
T3CONbits.TON =1; // Pone en marcha el Timer 1
}
void esperar(unsigned int t){
    /*¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡
     * Esta  funcion solo  funciona  correctamente cuando
     * las interrupciones del TIMER2 estan deshabilitadas
     * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!   */
    TMR3 = 0; // Pone a 0 el timer para esperar a la interrupcion (Asegura que tarde siempre lo establecido en Inic_Timer_LCD)
    unsigned int cont=0;
    while(cont!=t){ //Multiplicador para esperar el periodo 't' veces (ej.:1 vez 40us. 41 veces 1.64ms).
        while(!_TLCDIF){} //Esperar a la interrupcion
        cont++;
    }

    _TLCDIF = 0;
}
void Inic_LCD(void){
    Inic_Timer_LCD(40000); //40us == 40.000 ns
    RS = 0;
    RW = 1;
    E = 1;

    RW_TRIS = 0;
    RS_TRIS = 0;
    E_TRIS = 0;

    DATA_LCD_LAT &= 0xFF00;
    DATA_LCD_TRIS &= 0xFF00;

    esperar(375);
    function_set;
    esperar(125);//espera 5ms (125*40us)
    function_set;
    esperar(1);//espera 40ms (1000*40us)
    function_set;
    esperar(1);//espera 40ms (1000*40us)
    display_on();
    esperar(1);//espera 40ms (1000*40us)
    entry_mode();
    esperar(1);

}

//========================= OPERACIONES DE SOBRE LCD ===========================
void lcd_cmd(unsigned char cmd){
    DATA_LCD_LAT &= 0xFF00; // Deja los 8 bits de más peso con su valor intacto
    DATA_LCD_LAT |= cmd;
    RS = 0;
    RW = 0;
    E = 1;
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    E = 0;
    RW = 1;
}
void lcd_data(unsigned char dato){
    DATA_LCD_LAT &= 0xFF00; // Deja los 8 bits de más peso con su valor intacto
    DATA_LCD_LAT |= dato;
    RS = 1;
    RW = 0;
    E = 1;
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    E = 0;
    RW = 1;
}

//======================= OPERACIONES DE SOBRE VENATANA ========================
void display_upper_line(){
    if(display_in_sec_line>0)
        display_in_sec_line--;
    else
        display_in_sec_line=LINES-2;
}
void display_lower_line(){
    if(display_in_sec_line<LINES-2)
        display_in_sec_line++;
    else
        display_in_sec_line=0;
}
void display_next_servo(){
    if(display_in_serv_line<8)
        display_in_serv_line++;
    else
        display_in_serv_line=0;
}
void display_prev_servo(){
    if(display_in_serv_line>0)
        display_in_serv_line--;
    else
        display_in_serv_line=8;
}

char get_char_ventana(int v,int h){
    return ventana_LCD[v][h];
}
void set_char_linea_1(unsigned char c, int pos) {
    ventana_LCD[0][pos] = c;
}
void set_char_linea_2(unsigned char c, int pos) {
    ventana_LCD[1][pos] = c;
}
void set_char_ventana(unsigned char c,int pari, int parj) {
    ventana_LCD[pari][parj] = c;
}
void set_string_ventana(char *c,int pari, int parj) {
    int jj;
    for(jj=0; jj+parj<16; jj++){
        if(c[jj]=='\0')break;
        ventana_LCD[pari][jj+parj] = c[jj];
    }
}
void set_int_ventana_1Cifra(int val1, int offi, int offj){
    set_char_ventana((char)((val1%10+48))       , offi, offj); // unidades

}
void set_int_ventana_2Cifras(int val1, int offi, int offj){
    set_char_ventana((char)(((val1/10)%10)+48)  , offi, offj); // Decenas
    set_char_ventana((char)((val1%10+48))       , offi, offj+1); // unidades

}
void set_int_ventana_3Cifras(int val1, int offi, int offj){
    set_char_ventana((char)(((val1/100)%10)+48) , offi, offj); // Centenas
    set_char_ventana((char)(((val1/10)%10)+48)  , offi, offj+1); // Decenas
    set_char_ventana((char)((val1%10+48))       , offi, offj+2); // unidades

}
void set_int_ventana_4Cifras(int val1, int offi, int offj){
    set_char_ventana((char)(((val1/1000)%10)+48), offi, offj);   // Millares
    set_char_ventana((char)(((val1/100)%10)+48) , offi, offj+1); // Centenas
    set_char_ventana((char)(((val1/10)%10)+48)  , offi, offj+2); // Decenas
    set_char_ventana((char)((val1%10+48))       , offi, offj+3); // unidades

}

void set_char_ven_serv(unsigned char c,int pari, int parj) {
    ventana_servos[pari][parj] = c;
}
void set_string_ven_serv(char *c,int pari, int parj) {
    int jj;
    for(jj=0; jj+parj<16; jj++){
        if(c[jj]=='\0')break;
        ventana_servos[pari][jj+parj] = c[jj];
    }
}
//void set_int_ven_serv_1Cifra(int val1, int offi, int offj){
//    set_char_ventana((char)((val1%10+48))       , offi, offj); // unidades
//
//}
//void set_int_ven_serv_2Cifras(int val1, int offi, int offj){
//    set_char_ventana((char)(((val1/10)%10)+48)  , offi, offj); // Decenas
//    set_char_ventana((char)((val1%10+48))       , offi, offj+1); // unidades
//
//}
//void set_int_ven_serv_3Cifras(int val1, int offi, int offj){
//    set_char_ventana((char)(((val1/100)%10)+48) , offi, offj); // Centenas
//    set_char_ventana((char)(((val1/10)%10)+48)  , offi, offj+1); // Decenas
//    set_char_ventana((char)((val1%10+48))       , offi, offj+2); // unidades
//
//}
void set_int_ven_serv_4Cifras(int val1, int offi, int offj){
    set_char_ven_serv((char)(((val1/1000)%10)+48), offi, offj);   // Millares
    set_char_ven_serv((char)(((val1/100)%10)+48) , offi, offj+1); // Centenas
    set_char_ven_serv((char)(((val1/10)%10)+48)  , offi, offj+2); // Decenas
    set_char_ven_serv((char)((val1%10+48))       , offi, offj+3); // unidades

}

void update_serv_line(){
    switch(display_in_serv_line){
        case 0:
            set_int_ven_serv_4Cifras(OC1RS,display_in_serv_line,10);
            break;
        case 1:
            set_int_ven_serv_4Cifras(OC2RS,display_in_serv_line,10);
            break;
        case 2:
            set_int_ven_serv_4Cifras(OC3RS,display_in_serv_line,10);
            break;
        case 3:
            set_int_ven_serv_4Cifras(OC4RS,display_in_serv_line,10);
            break;
        case 4:
            set_int_ven_serv_4Cifras(S_OC1RS,display_in_serv_line,10);
            break;
        case 5:
            set_int_ven_serv_4Cifras(S_OC2RS,display_in_serv_line,10);
            break;
        case 6:
            set_int_ven_serv_4Cifras(S_OC3RS,display_in_serv_line,10);
            break;
        case 7:
            set_int_ven_serv_4Cifras(S_OC4RS,display_in_serv_line,10);
            break;
        case 8:
            set_int_ven_serv_4Cifras(S_OC5RS,display_in_serv_line,10);
            break;
    }
    set_string_ventana(ventana_servos[display_in_serv_line],7,0);
}

//============================= RUTINA DE ATENCION =============================
void _ISR _T_LCD_Interrupt(void){
// else if(global == RENDER){//PARA BORRAR//
     /* Fase de refrescado del LCD:
      *     Se vuelca la  ventana  de 2x16  en
      *   la que se  habran hecho  los cambios
      *   pertinentes, previamente en UPDATE.
      *     Esto  se  hace a traves  de cuatro
      *   estados, recorrer  los cuales  lleva
      *   34 interrupciones de reloj.
      */
    switch(render){
        case SEL_LINE_1:
            line_1();
            render=WR_LINE_1; // Va a escribir linea 1.
            break;
        case SEL_LINE_2:
            line_2();
            render=WR_LINE_2; // Va a escribir linea 2.
            break;
        case WR_LINE_1:
            lcd_data(ventana_LCD [0][f]);
            //putRS232_2(ventana_LCD [0][i]);
            f++;
            if(f>15){
                f=0;
                render=SEL_LINE_2; // Ha terminado de escribir linea 1. Va seleccionar linea 2.
            }
            break;
        case WR_LINE_2:
            lcd_data(ventana_LCD [display_in_sec_line+1][f]);
            //putRS232_2(ventana_LCD [1][i]);
            f++;
            if(f>15){
                f=0;
                render=SEL_LINE_1; // Ha terminado de escribir linea 2. Va seleccionar linea 1 pero
            }
            break;
        default:
            render=SEL_LINE_1;
    }
// }
 _TLCDIF = 0;
}