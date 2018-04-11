/*
 * Proyecto: DSE_Grupo_6_IO_v7
 * Fichero: CAD_G6_v7.h
 *
 * Grupo: G6    Autor: Cristian Mares e Imanol Atienza
 * v7.0	Fecha: 27-1-2016
 */

//Temporizador del CAD
#define CAD_Timer 4 // Define el Timer a usar

#if(CAD_Timer == 1)
 #define _T_CAD_Interrupt _T1Interrupt
 #define _TcadIF _T1IF
 #define _TcadEI _T1IE
#elif(CAD_Timer == 2)
 #define _T_CAD_Interrupt _T2Interrupt
 #define _TcadIF _T2IF
 #define _TcadEI _T2IE
#elif(CAD_Timer == 3)
 #define _T_CAD_Interrupt _T3Interrupt
 #define _TCADIF _T3IF
 #define _TCADEI _T3IE
#elif(CAD_Timer == 4)
 #define _T_CAD_Interrupt _T4Interrupt
 #define _TCADIF _T4IF
 #define _TCADEI _T4IE
#elif(CAD_Timer==5)
 #define _T_CAD_Interrupt _T5Interrupt
 #define _TcadIF _T5IF
 #define _TcadEI _T5IE
#elif(CAD_Timer==6)
 #define _T_CAD_Interrupt _T6Interrupt
 #define _TcadIF _T6IF
 #define _TcadEI _T6IE
#elif(CAD_Timer==7)
 #define _T_CAD_Interrupt _T7Interrupt
 #define _TcadIF _T7IF
 #define _TcadEI _T7IE
#elif(CAD_Timer==8)
 #define _T_CAD_Interrupt _T8Interrupt
 #define _TcadIF _T8IF
 #define _TcadEI _T8IE
#elif(CAD_Timer==9)
 #define _T_CAD_Interrupt _T9Interrupt
 #define _TcadIF _T9IF
 #define _TcadEI _T9IE
#elif(CAD_Timer == 0 || CAD_Timer > 9)
#error ("TIMER PARA CAD NO DEFINIDO")
#endif

void Inic_CAD(void);
void switch_estCalibrar(void);