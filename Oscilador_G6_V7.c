
/*
 * Proyecto: DSE_Grupo_6_IO_v7
 * Fichero: Oscilador_G6_v7.h
 *
 * Grupo: G6    Autor: Cristian Mares e Imanol Atienza
 * v7.0	Fecha: 27-1-2016
 *
 * Selecciona la frecuencia del Oscilador Principal
 *
 * Solo una referencia de FOSSCEL, FOSC, FWDT debe aparecer en un proyecto
 * al inicio de un fichero fuente C, fuera de ninguna función
 *
 * Oscillator Source Selection:
 * FNOSC_FRC Fast RC oscillator
 * FNOSC_FRCPLL Fast RC oscillator w/ divide and PLL
 * FNOSC_PRI Primary oscillator (XT, HS, EC)
 * FNOSC_PRIPLL Primary oscillator (XT, HS, EC) w/ PLL
 * FNOSC_SOSC Secondary oscillator
 * FNOSC_LPRC Low power RC oscillator
 * FNOSC_FRCDIv76 Fast RC oscillator w/ divide by 16
 * FNOSC_LPRCDIVN Low power Fast RC oscillator w/divide by N
 *
 * Clock switching and clock monitor:
 * FCKSM_CSECME Both enabled
 * FCKSM_CSECMD Only clock switching enabled
 * FCKSM_CSDCMD Both disabled
 *
 * OSC2 Pin function:
 * OSCIOFNC_ON Digital I/O
 * OSCIOFNC_OFF OSC2 is clock O/P
 *
 * Oscillator Selection:
 * POSCMD_EC External clock
 * POSCMD_XT XT oscillator
 * POSCMD_HS HS oscillator
 * POSCMD_NONE Primary disable
*/
#include "p24hj256gp610A.h"
#include "Oscilador_G6_v7.h"

//_FOSCSEL (FNOSC_PRIPLL); // Primary oscillator (XT, HS, EC) w/ PPL
//_FOSC (FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);
 // Clock switching and clock monitor: Both
 // disable, OSC2 is clock O/P,XT oscillator
//_FWDT (FWDTEN_OFF); // Watchdog Timer:Disabled
//==============================================================
void Inic_Oscilador(void)
{
// Configure Oscillator to operate the device at 40MHz
// Fosc= Fin*M(N1*N2), Fcy=Fosc/2
// Fosc= 8M*40/(2*2)= 80MHz for 8M input clock
// M=40 --> 80 MHz Instruccion 25ns
// M=20 --> 40 MHz Instruccion 50ns
// M=10 --> 20 MHz Instruccion 100ns
 PLLFBD = 40-2;
 CLKDIVbits.PLLPOST = 0;//--> N2=2
 CLKDIVbits.PLLPRE = 0;//--> N1=2
// OSCTUN = 0; // TUNE FRC oscillator, if FRC is used
 RCONbits.SWDTEN = 0; // Disable Watch Dog Timer
// Funciones pre-compiladas: MPLAB C30 -DS51284G- apendice B
// Clock switch to incorporate PLL
 __builtin_write_OSCCONH(0x03); // Initiate Clock Switch to Primary
 // Oscillator with PLL (NOSC=0b011)
 __builtin_write_OSCCONL(0x01); // Start clock switching
 while (OSCCONbits.COSC != 0b011); // Wait fot Clock switch to occur
// Wait for PLL to lock
 while(OSCCONbits.LOCK !=1) {}
} // === Fin Inic_Oscilator ===

