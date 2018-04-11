/*
Fichero: Crono_G6_v7.h

Grupo: 6    Autor: Imanol y Cristian
v7.0	Fecha: 15-2-2016

// Asigna la función de interrupción correspondiente al timer seleccionado al Crono
// Asigna el flag de interrupción correspondiente al timer seleccionado al Crono
// Si la asignación no es correcta genera un mensaje de error
*/
#include "p24hj256gp610A.h"
#define Crono_Timer 1 // Define el Timer a usar

#if(Crono_Timer == 1)
 #define _T_Crono_Interrupt _T1Interrupt
 #define _TcronoIF _T1IF
 #define _TcronoEI _T1IE
#elif(Crono_Timer == 2)
 #define _T_Crono_Interrupt _T2Interrupt
 #define _TcronoIF _T2IF
 #define _TcronoEI _T2IE
#elif(Crono_Timer == 3)
 #define _T_Crono_Interrupt _T3Interrupt
 #define _TcronoIF _T3IF
 #define _TcronoEI _T3IE
#elif(Crono_Timer == 4)
 #define _T_Crono_Interrupt _T4Interrupt
 #define _TcronoIF _T4IF
 #define _TcronoEI _T4IE
#elif(Crono_Timer==5)
 #define _T_Crono_Interrupt _T5Interrupt
 #define _TcronoIF _T5IF
 #define _TcronoEI _T5IE
#elif(Crono_Timer==6)
 #define _T_Crono_Interrupt _T6Interrupt
 #define _TcronoIF _T6IF
 #define _TcronoEI _T6IE
#elif(Crono_Timer==7)
 #define _T_Crono_Interrupt _T7Interrupt
 #define _TcronoIF _T7IF
 #define _TcronoEI _T7IE
#elif(Crono_Timer==8)
 #define _T_Crono_Interrupt _T8Interrupt
 #define _TcronoIF _T8IF
 #define _TcronoEI _T8IE
#elif(Crono_Timer==9)
 #define _T_Crono_Interrupt _T9Interrupt
 #define _TcronoIF _T9IF
 #define _TcronoEI _T9IE
#elif(Crono_Timer == 0 || Crono_Timer >= 9)
#error ("TIMER PARA CRONO NO DEFINIDO")
#endif

//Inicializar Timer 1 para el Cronometro
void Inic_Timer_1(void); //

//=== Control del cronometro ===
void ini_Crono(void);
void pause_Crono(void);
void stop_Crono(void);
//====== Recoger info. del Cronometro ======
    void get_mseg(unsigned char *);
    void get_seg(unsigned char *);
    void get_min(unsigned char *);

    unsigned int get_mSeg();
    unsigned int get_cSeg();
    unsigned int get_dSeg();
    unsigned int get_uSeg();
    unsigned int get_DSeg();
    unsigned int get_uMin();
    unsigned int get_DMin();
//=== Actualizar ventana_LCD ===
void update_crono_line(void);