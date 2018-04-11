/* 
 * File:   SRF08_G6_v7.h
 * Author: ehu
 *
 * Created on 7 de abril de 2016, 13:54
 */
#include "p24hj256gp610A.h"

#define SRF08_Timer 5 // Define el Timer a usar

#if(SRF08_Timer == 1)
 #define _T_Crono_Interrupt _T1Interrupt
 #define _TcronoIF _T1IF
 #define _TcronoEI _T1IE
#elif(SRF08_Timer == 2)
 #define _T_Crono_Interrupt _T2Interrupt
 #define _TcronoIF _T2IF
 #define _TcronoEI _T2IE
#elif(SRF08_Timer == 3)
 #define _T_Crono_Interrupt _T3Interrupt
 #define _TcronoIF _T3IF
 #define _TcronoEI _T3IE
#elif(SRF08_Timer == 4)
 #define _T_Crono_Interrupt _T4Interrupt
 #define _TcronoIF _T4IF
 #define _TcronoEI _T4IE
#elif(SRF08_Timer==5)
 #define _T_SRF08_Interrupt _T5Interrupt
 #define _TSRF08IF _T5IF
 #define _TSRF08EI _T5IE
#elif(SRF08_Timer==6)
 #define _T_Crono_Interrupt _T6Interrupt
 #define _TcronoIF _T6IF
 #define _TcronoEI _T6IE
#elif(SRF08_Timer==7)
 #define _T_Crono_Interrupt _T7Interrupt
 #define _TcronoIF _T7IF
 #define _TcronoEI _T7IE
#elif(SRF08_Timer==8)
 #define _T_Crono_Interrupt _T8Interrupt
 #define _TcronoIF _T8IF
 #define _TcronoEI _T8IE
#elif(SRF08_Timer==9)
 #define _T_Crono_Interrupt _T9Interrupt
 #define _TcronoIF _T9IF
 #define _TcronoEI _T9IE
#elif(SRF08_Timer == 0 || SRF08_Timer >= 9)
#error ("TIMER PARA CRONO NO DEFINIDO")
#endif
void Inic_SFR08(void);
void Inic_Timer_5(void);
void inic_measure(void);
void read_measure_sensor(void);
void change_dirI2C(unsigned char,unsigned char);
void read_dirI2C(unsigned char);
char get_sensor_data(int);
int get_distance(void) ;
char i2c_read(void);
void i2c_set_read(char v);
