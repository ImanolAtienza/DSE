                                                              
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;	Filename:			    i2c.h            
;	Date:				    February 21, 2007          
;	File Version:		  	1.0                             
;	Assembled using:		MPLAB IDE 7.51.00.0               
; 	Author:		  	    	Martin Bowman              
;	Company:			    Microchip Technology, Inc.
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

//This file contains the function prototypes for the i2c function
#define PAGESIZE	32

//Low Level Functions
unsigned int IdleI2C_1(void);
unsigned int StartI2C_1(void);
unsigned int WriteI2C_1(unsigned char);
unsigned int StopI2C_1(void);
unsigned int RestartI2C_1(void);
unsigned int getsI2C_1(unsigned char*, unsigned char);
unsigned int NotAckI2C_1(void);
unsigned int InitI2C_1(void);
unsigned int ACKStatus_1(void);
unsigned int getI2C_1(void);
unsigned int AckI2C_1(void);
unsigned int EEAckPolling_1(unsigned char);
unsigned int putstringI2C_1(unsigned char*);

//High Level Functions for Low Density Devices
unsigned int LDByteReadI2C_1
    (unsigned char, unsigned char, unsigned char*, unsigned char);

unsigned int LDByteWriteI2C_1
    (unsigned char, unsigned char, unsigned char);

unsigned int LDPageWriteI2C_1
    (unsigned char, unsigned char, unsigned char*);

unsigned int LDSequentialReadI2C_1
     (unsigned char, unsigned char,  unsigned char*, unsigned char);

//High Level Functions for High Density Devices
unsigned int HDByteReadI2C_1
   (unsigned char, unsigned char, unsigned char, unsigned char*, unsigned char);

unsigned int HDByteWriteI2C_1
    (unsigned char, unsigned char, unsigned char, unsigned char);

unsigned int HDPageWriteI2C_1
    (unsigned char, unsigned char, unsigned char, unsigned char*);

unsigned int HDSequentialReadI2C_1
   (unsigned char, unsigned char, unsigned char, unsigned char*, unsigned char);

