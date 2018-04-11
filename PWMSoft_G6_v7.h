/* 
 * File:   PWMSoft_G6_v7.h
 * Author: ehu
 *
 * Created on 27 de abril de 2016, 11:33
 */

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

#define  T_servos_COM   T2CON
#define  T_servos_COMbits   T2CONbits
#define PR_servos   PR2
#define TMR_servos  TMR2
#define _T_servos_IF    _T2IF

//#define PWMSoft_Timer 6 // Define el Timer a usar
//
//#if(PWMSoft_Timer == 1)
// #define _T_PWMSoft_Interrupt _T1Interrupt
// #define _TPWMSoftIF _T1IF
// #define _TPWMSoftEI _T1IE
//#elif(PWMSoft_Timer == 2)
// #define _T_PWMSoft_Interrupt _T2Interrupt
// #define _TPWMSoftIF _T2IF
// #define _TPWMSoftEI _T2IE
//#elif(PWMSoft_Timer == 3)
// #define _T_PWMSoft_Interrupt _T3Interrupt
// #define _TPWMSoftIF _T3IF
// #define _TPWMSoftEI _T3IE
//#elif(PWMSoft_Timer == 4)
// #define _T_PWMSoft_Interrupt _T4Interrupt
// #define _TPWMSoftIF _T4IF
// #define _TPWMSoftEI _T4IE
//#elif(PWMSoft_Timer==5)
// #define _T_SRF08_Interrupt _T5Interrupt
// #define _TSRF08IF _T5IF
// #define _TSRF08EI _T5IE
//#elif(PWMSoft_Timer==6)
// #define _T_PWMSoft_Interrupt _T6Interrupt
// #define _TPWMSoftIF _T6IF
// #define _TPWMSoftEI _T6IE
//#elif(PWMSoft_Timer==7)
// #define _T_PWMSoft_Interrupt _T7Interrupt
// #define _TPWMSoftIF _T7IF
// #define _TPWMSoftEI _T7IE
//#elif(PWMSoft_Timer==8)
// #define _T_PWMSoft_Interrupt _T8Interrupt
// #define _TPWMSoftIF _T8IF
// #define _TPWMSoftEI _T8IE
//#elif(PWMSoft_Timer==9)
// #define _T_PWMSoft_Interrupt _T9Interrupt
// #define _TPWMSoftIF _T9IF
// #define _TPWMSoftEI _T9IE
//#elif(PWMSoft_Timer == 0 || PWMSoft_Timer >= 9)
//#error ("TIMER PARA CRONO NO DEFINIDO")
//#endif
