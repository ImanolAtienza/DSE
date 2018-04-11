/*
 * Proyecto: DSE_Grupo_6_IO_v7
 * Fichero: LCD_Explorer16_24H_G6_v7.h
 *
 * Grupo: G6    Autor: Cristian Mares e Imanol Atienza
 * v7.0	Fecha: 27-1-2016
 */


#include "p24hj256gp610A.h"


// Patas utilizadas por el LCD en la Explorer 16
//   RS      -> Rx
//   E        -> Rx
//   RW    -> Rx
//   DATA -> Rx ... Rx
// Define los pines
#define  RW  _LATD5        // LCD R/W
#define  RS  _LATB15        // LCD RS
#define  E   _LATD4        // LCD E        // Puerto de datos para LCD
// Define las direcciones de los pines
#define  RW_TRIS _TRISD5
#define  RS_TRIS _TRISB15
#define  E_TRIS  _TRISD4          //

#define DATA_LCD_LAT LATE
#define DATA_LCD_TRIS TRISE

//Temporizador del LCD
#define LCD_Timer 3 // Define el Timer a usar

#if(LCD_Timer == 1)
 #define _T_Crono_Interrupt _T1Interrupt
 #define _TcronoIF _T1IF
 #define _TcronoEI _T1IE
#elif(LCD_Timer == 2)
 #define _T_LCD_Interrupt _T2Interrupt
 #define _TLCDIF _T2IF
 #define _TLCDIE _T2IE
#elif(LCD_Timer == 3)
 #define _T_LCD_Interrupt _T3Interrupt
 #define _TLCDIF _T3IF
 #define _TLCDIE _T3IE
#elif(LCD_Timer == 4)
 #define _T_Crono_Interrupt _T4Interrupt
 #define _TcronoIF _T4IF
 #define _TcronoEI _T4IE
#elif(LCD_Timer==5)
 #define _T_Crono_Interrupt _T5Interrupt
 #define _TcronoIF _T5IF
 #define _TcronoEI _T5IE
#elif(LCD_Timer==6)
 #define _T_Crono_Interrupt _T6Interrupt
 #define _TcronoIF _T6IF
 #define _TcronoEI _T6IE
#elif(LCD_Timer==7)
 #define _T_Crono_Interrupt _T7Interrupt
 #define _TcronoIF _T7IF
 #define _TcronoEI _T7IE
#elif(LCD_Timer==8)
 #define _T_Crono_Interrupt _T8Interrupt
 #define _TcronoIF _T8IF
 #define _TcronoEI _T8IE
#elif(LCD_Timer==9)
 #define _T_Crono_Interrupt _T9Interrupt
 #define _TcronoIF _T9IF
 #define _TcronoEI _T9IE
#elif(LCD_Timer == 0 || LCD_Timer > 9)
#error ("TIMER PARA CRONO NO DEFINIDO")
#endif

//========================= OPERACIONES DE SOBRE LCD ===========================
void lcd_cmd(unsigned char cmd);//Escribe (envía) un comando al lcd
void lcd_data(unsigned char data);//Escribe (envía) un dato al lcd

/***** LCD COMMAND FUCNTION PROTOTYPES*****/
#define function_set        lcd_cmd(0x38)//40us
#define display_on()        lcd_cmd(0x0C)//40us
#define entry_mode()        lcd_cmd(0x06)//40us
#define line_1()            lcd_cmd(0x80)//40us, DDRAM 0x00, cursor inicio fila 1
#define line_2()            lcd_cmd(0xC0)//40us, DDRAM 0x40, cursor inicio fila 2

//#define cursor_right()    lcd_cmd(0x14)//40us
//#define cursor_left()     lcd_cmd(0x10)//40us
//#define display_shift()   lcd_cmd(0x1C)//40us
//#define home_clr()        lcd_cmd(0x01)//1,64ms
//#define home_it()         lcd_cmd(0x02)//1,64ms

//====================== OPERACIONES DE INICIALIZACION =========================
void copiar_FLASH_RAM(const unsigned char *texto);
void Inic_LCD(void);// Inicializa display
void esperar(unsigned int t); //Solo para cuando las interrupciones estan desactivadas


//======================= OPERACIONES DE SOBRE VENATANA ========================
void display_upper_line(void);
void display_lower_line(void);
void display_next_servo(void);
void display_prev_servo(void);

char get_char_ventana(int, int);
void set_char_linea_1(unsigned char, int);
void set_char_linea_2(unsigned char, int);
void set_char_ventana(unsigned char, int, int);
void set_string_ventana(char*, int, int);
void set_int_ventana_1Cifra(int,int,int);
void set_int_ventana_2Cifras(int,int,int);
void set_int_ventana_3Cifras(int,int,int);
void set_int_ventana_4Cifras(int,int,int);

void set_char_ven_serv(unsigned char, int, int);
void set_string_ven_serv(char*, int, int);
//void set_int_ventana_1Cifra(int,int,int);
//void set_int_ventana_2Cifras(int,int,int);
//void set_int_ventana_3Cifras(int,int,int);
void set_int_ven_serv_4Cifras(int,int,int);

void update_serv_line(void);