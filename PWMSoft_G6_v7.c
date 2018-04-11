#include "p24hj256gp610A.h"
#include "PWM_G6_v7.h"

extern unsigned int valorPotenciometro;



/*
 80MHz -> 40Mhz Cy -> 25ns
 * Prescler 1:1
 * 1 us -> 40 instruc. 1 ms -> 40.000 Inst. 20ms -> 800.000 inst.
 * Prescaler 1:16
 * 1 incremento -> 25 x 64 = 1600 ns -> 1ms -> 1000000/1600 = 625    20ms -> 12500
 * 0,3ms -> 188  1,3 -> 812  2,3ms-> 1437
 *
 */



void Inic_Servos_software (void)
{
    T_servos_COM = 0;
    T_servos_COMbits.TCKPS =  2;      // Prescaler 1:256, 1:64, 1:8, 1:1
    PR_servos = duty_20msg;    // Periodo

    Lata_SerSoft1 = 0;
    Lata_SerSoft2 = 0;
    Lata_SerSoft3 = 0;
    Lata_SerSoft4 = 0;
    Lata_SerSoft5 = 0;

    Servo_1_TRIS = 0;
    Servo_2_TRIS = 0;
    Servo_3_TRIS = 0;
    Servo_4_TRIS = 0;
    Servo_5_TRIS = 0;

    servo1_duty = duty_medio;
    servo2_duty = duty_medio;
    servo3_duty = duty_medio;
    servo4_duty = duty_medio;
    servo5_duty = duty_medio;

    ciclo_servos = 1;

    _T3IF = 0;
    _T3IE = 1;

    T_servos_COMbits.TON = 1;
    TRIS_LED_D5 = 0;
}

void _ISR _T2Interrupt( void)
{
//    if (Estado_LCD== 2) LCD_Incio_Secuencia_Int ();	//LCD

    _T2IF = 0;	//
} //    FIN _ISR _T2Interrupt
*/


/*
 80MHz -> 40Mhz Cy -> 25ns
 * Prescler 1:1
 * 1 us -> 40 instruc. 1 ms -> 40.000 Inst. 20ms -> 800.000 inst.
 * Prescaler 1:16
 * 1 incremento -> 25 x 64 = 1600 ns -> 1ms -> 1000000/1600 = 625    20ms -> 12500
 * 0,3ms -> 188  1,3 -> 812  2,3ms-> 1437
 *
 */

#define _T_servo_Interrupt _T2Interrupt
#define  T_servos_COM   T2CON
#define  T_servos_COMbits   T2CONbits
#define PR_servos   PR2
#define TMR_servos  TMR2
#define _T_servos_IF    _T2IF

//INTERRUPCION CONTROL SERVOS
// Genera las señales de 8 servos (protocolo FUTABA?)
void _ISR _T_servo_Interrupt (void)
{
//Rebose del timer
LED_D5 = !LED_D5;
        switch (ciclo_servos)
    {
	case 1 :	//Cargamos valor  servo 1
	    Servo_1_LAT = 1; //activamos señal

//            servo1_duty = 188 + valorPotenciometro*((Servo_max_float - Servo_min_float)/(Pot_max_float - Pot_min_float));
            PR_servos = servo1_duty;
	    TMR_servos = 0;
            T_servos_COMbits.TON = 1;
	    ciclo_servos = 2;
            break;
	case 2 :	//Cargamos valor 2º servo
	    Servo_1_LAT = 0; //desactivamos señal
 	    Servo_2_LAT = 1; //activamos señal
//            servo2_duty = 188 + valorPotenciometro*((Servo_max_float - Servo_min_float)/(Pot_max_float - Pot_min_float));
 	    PR_servos = servo2_duty;

	    TMR_servos = 0;
            T_servos_COMbits.TON = 1;
	    ciclo_servos = 3;
            break;
 	case 3 :	//Cargamos valor primer servo
	    Servo_2_LAT = 0; //desactivamos señal
 	    Servo_3_LAT = 1; //activamos señal
//            servo3_duty = 188 + valorPotenciometro*((Servo_max_float - Servo_min_float)/(Pot_max_float - Pot_min_float));
 	    PR_servos = servo3_duty;
	    TMR_servos = 0;
            T_servos_COMbits.TON = 1;
	    ciclo_servos = 4;
	    break;

   	case 4 :	//Cargamos valor primer servo
	    Servo_3_LAT = 0; //desactivamos señal
 	    Servo_4_LAT = 1; //activamos señal
	    PR_servos = servo4_duty;
	    TMR_servos = 0;
//             servo4_duty = 188 + valorPotenciometro*((Servo_max_float - Servo_min_float)/(Pot_max_float - Pot_min_float));
           T_servos_COMbits.TON = 1;
	    ciclo_servos = 5;
	    break;
	case 5 :	//Cargamos valor primer servo
	    Servo_4_LAT = 0; //desactivamos señal
 	    Servo_5_LAT = 1; //activamos señal
             servo5_duty = 188 + valorPotenciometro*((Servo_max_float - Servo_min_float)/(Pot_max_float - Pot_min_float));
  	    PR_servos = servo5_duty;
	    TMR_servos = 0;
            T_servos_COMbits.TON = 1;
	    ciclo_servos = 6;
	    break;
	case 6 :	//Cargamos valor primer servo
	    Servo_5_LAT = 0; //desactivamos señal
	    Servo_4_LAT = 0; //desactivamos señal
	    Servo_3_LAT = 0; //desactivamos señal
	    Servo_2_LAT = 0; //desactivamos señal
	    Servo_1_LAT = 0; //desactivamos señal

            PR_servos = duty_20msg - (servo1_duty +servo2_duty +servo3_duty +servo4_duty +servo5_duty);
	    TMR_servos = 0;
            T_servos_COMbits.TON = 1;
	    ciclo_servos = 1;
	    break;
    }

    _T_servos_IF = 0;
} //FIN de _ISR _T5Interrupt

void _ISR _T_PWMSoft_Interrupt( void) {
//    if (Estado_LCD== 2) LCD_Incio_Secuencia_Int ();	//LCD

    _T6IF = 0;	//
}
