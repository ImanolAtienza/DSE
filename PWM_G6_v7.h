/* 
 * File:   PWM_G6_v7.h
 * Author: Christian Mares y Emanuel Atienza
 *
 * Created on 31 de marzo de 2016, 12:37
 */

#include "p24hj256gp610A.h"

#define Lata_SerSoft1 _LATB10
#define Lata_SerSoft2 _LATB11
#define Lata_SerSoft3 _LATB12
#define Lata_SerSoft4 _LATB13
#define Lata_SerSoft5 _LATB14

#define Trisa_SerSoft1 _TRISB10
#define Trisa_SerSoft2 _TRISB11
#define Trisa_SerSoft3 _TRISB12
#define Trisa_SerSoft4 _TRISB13
#define Trisa_SerSoft5 _TRISB14

//Temporizador del PWM
#define PWD_Timer 2 // Define el Timer a usar

#if(PWD_Timer == 1)
 #define _T_Crono_Interrupt _T1Interrupt
 #define _TcronoIF _T1IF
 #define _TcronoEI _T1IE
#elif(PWD_Timer == 2)
 #define _T_PWM_Interrupt _T2Interrupt
 #define _TPWMIF _T2IF
 #define _TPWMIE _T2IE
#elif(PWD_Timer == 3)
 #define _T_LCD_Interrupt _T3Interrupt
 #define _TLCDIF _T3IF
 #define _TLCDIE _T3IE
#elif(PWD_Timer == 4)
 #define _T_Crono_Interrupt _T4Interrupt
 #define _TcronoIF _T4IF
 #define _TcronoEI _T4IE
#elif(PWD_Timer==5)
 #define _T_Crono_Interrupt _T5Interrupt
 #define _TcronoIF _T5IF
 #define _TcronoEI _T5IE
#elif(PWD_Timer==6)
 #define _T_Crono_Interrupt _T6Interrupt
 #define _TcronoIF _T6IF
 #define _TcronoEI _T6IE
#elif(PWD_Timer==7)
 #define _T_Crono_Interrupt _T7Interrupt
 #define _TcronoIF _T7IF
 #define _TcronoEI _T7IE
#elif(PWD_Timer==8)
 #define _T_Crono_Interrupt _T8Interrupt
 #define _TcronoIF _T8IF
 #define _TcronoEI _T8IE
#elif(PWD_Timer==9)
 #define _T_Crono_Interrupt _T9Interrupt
 #define _TcronoIF _T9IF
 #define _TcronoEI _T9IE
#elif(PWD_Timer == 0 || PWD_Timer > 9)
#error ("TIMER PARA CRONO NO DEFINIDO")
#endif

void Inic_PWM(void);
void Inic_Timer_PWM(int);

void calibrar_servo(int);

void center_Servo(int);
void girar_Drch(int);
void girar_Izq(int);
void girar(int ,int );
void set_position(int,int);

void reset_servs();
void un_movimiento_sexy(void);