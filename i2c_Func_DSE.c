/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;   Programa modificado de i2c_Func.c  ;February 21, 2007   ;Martin Bowman
;                                                                
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;	Filename:			    i2c_Func_DSE.c            
;	Date:				    Marzo, 2012          
;	File Version:		  	1.1                             
;	Assembled using:		MPLAB IDE 8.83.00.0               
; 	Author:		  	    	Luis G               
;	Company:			    
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include <p24HJ256GP610A.h>
#include "i2c_Func_DSE.h"

#define PAGESIZE	32

//DECLARACIONES :
unsigned int ACKStatus_1(void);
unsigned int NotAckI2C_1(void);
unsigned int getsI2C_1(unsigned char *rdptr, unsigned char Length);
unsigned int EEAckPolling_1(unsigned char control);
unsigned int putstringI2C_1(unsigned char *wrptr);
unsigned int getI2C_1(void);

unsigned int Error_I2C_1;
// Error_I2C_1 = 

/*********************************************************************
* Function:      InitI2C_1()
* Input:            None.
* Output:           None.
* Overview:	Initialises the I2C(1) peripheral
* Note:             Sets up Master mode, No slew rate control, 400Khz

* This function will initialize the I2C(1) peripheral.

********************************************************************/
unsigned int InitI2C_1(void)
{	
//I2CBRG=(Fcy/Fscl)-(Fcy/10000000)-1	--> Fscl Frec reloj I2C
//  I2C1BRG = 395; 	//100kHz y Fcy= 40MHz //First set the I2C(1) BRG Baud Rate.
//  I2C1BRG = 95; 	//400kHz y Fcy= 40MHz --> Periodo = 2,5us
    I2C1BRG = 195; 	//400kHz y Fcy= 40MHz --> Periodo = 2,5us
	
    I2C1CON = 0x1200;	//Inicializa en modo Maestro, No Slew Rate, I2C desactivado
	
    I2C1RCV = 0x0000;	// Reg. recepción de datos
    I2C1TRN = 0x0000;	// Reg. transmisión de datos
	
    I2C1CONbits.I2CEN = 1;	//Habilita I2C
}

/*********************************************************************
* Function:        StartI2C_1()
* Input:		None.
* Output:		None.
* Overview:		Generates an I2C Start Condition
* Note:			None
********************************************************************/
unsigned int StartI2C_1(void)
{
unsigned int time_out;
//This function generates an I2C start condition and returns status of the Start.
    I2C1CONbits.SEN = 1;		//Generate Start Condition
    time_out= 0;
    while (I2C1CONbits.SEN & time_out<1000)	//Wait for Start Condition
    {
        time_out++;
    }
    //return(I2C1STATbits.S);	//Optionally return status
    if(time_out>=1000) return 1;
    return 0;
}

/*********************************************************************
* Function:        RestartI2C_1()
* Input:		None.
* Output:		None.
* Overview:		Generates a restart condition and optionally returns status
* Note:			None
********************************************************************/
unsigned int RestartI2C_1(void)
{
unsigned int time_out;
//This function generates an I2C Restart condition and returns status of the Restart.
    I2C1CONbits.RSEN = 1;		//Generate Restart
    time_out= 0;
    while (I2C1CONbits.RSEN & time_out<1000)	//Wait for restart
    {
    time_out++;
    }
    //return(I2C1STATbits.S);	//Optional - return status
    if(time_out>=1000) return 2;
    return 0;
}

/*********************************************************************
* Function:        StopI2C_1()
* Input:		None.
* Output:		None.
* Overview:		Generates a bus stop condition
* Note:			None
********************************************************************/
unsigned int StopI2C_1(void)
{
unsigned int time_out;
//This function generates an I2C stop condition and returns status of the Stop.
    I2C1CONbits.PEN = 1;		//Generate Stop Condition
    time_out= 0;
    while (I2C1CONbits.PEN & time_out<1000)	//Wait for Stop
    {
        time_out++;
    }
    //return(I2C1STATbits.P);	//Optional - return status
    if(time_out>=1000) return 3;
    return 0;
}

/*********************************************************************
* Function:        IdleI2C_1()
* Input:		None.
* Output:		None.
* Overview:		Waits for bus to become Idle
* Note:			None
********************************************************************/
unsigned int IdleI2C_1(void)
{
unsigned int time_out;
    time_out= 0;
    while (I2C1STATbits.TRSTAT & time_out<1000)		//Wait for bus Idle
    {
        time_out++;
    }
    if(time_out>=1000) return 5;
    return 0;
}

/*********************************************************************
* Function:        WriteI2C_1()
* Input:		Byte to write.
* Output:		None.
* Overview:		Writes a byte out to the bus
* Note:			None
********************************************************************/
unsigned int WriteI2C_1(unsigned char byte)
{
unsigned int time_out;
//This function transmits the byte passed to the function
    time_out= 0;
    while (I2C1STATbits.TRSTAT & time_out<1000)	//Wait for bus to be idle
    {
        time_out++;
    }
    if(time_out>=1000) Error_I2C_1=4;

    I2C1TRN = byte;             //Load byte to I2C1 Transmit buffer
    time_out= 0;
    while (I2C1STATbits.TBF & time_out<1000)	//wait for data transmission
    {
        time_out++;
    }
    if(time_out>=1000) return 4;
    return 0;
}

/*********************************************************************
* Function:        LDByteWriteI2C_1()
* Input:		Control Byte, 8 - bit address, data.
* Output:		None.
* Overview:		Write a byte to low density device at address LowAdd
* Note:			None
********************************************************************/
unsigned int LDByteWriteI2C_1(unsigned char ControlByte, unsigned char LowAdd, unsigned char data)
{
    unsigned int ErrorCode;
//Error_I2C_1=0 ;
    if (IdleI2C_1()) Error_I2C_1=5;
    if (StartI2C_1()) Error_I2C_1=1;
    if (WriteI2C_1(ControlByte)) Error_I2C_1=4;
    if (IdleI2C_1()) Error_I2C_1=5;

    ErrorCode = ACKStatus_1();		//Return ACK Status

    if (WriteI2C_1(LowAdd)) Error_I2C_1=4;
    if (IdleI2C_1()) Error_I2C_1=5;

    ErrorCode = ACKStatus_1();		//Return ACK Status

    if (WriteI2C_1(data)) Error_I2C_1=4;
    if (IdleI2C_1()) Error_I2C_1=5;
    if (StopI2C_1()) Error_I2C_1=3;

    return (Error_I2C_1);
}

/*********************************************************************
* Function:        LDByteReadI2C_1()
* Input:		Control Byte, Address, *Data, Length.
* Output:		None.
* Overview:		Performs a low density read of Length bytes and stores in *Data array
*				starting at Address.
* Note:			None
********************************************************************/
unsigned int LDByteReadI2C_1(unsigned char ControlByte, unsigned char Address, unsigned char *Data, unsigned char Length)
{
    unsigned int ErrorCode;
//Error_I2C_1=0 ;
    if (IdleI2C_1()) Error_I2C_1=5;
    if (StartI2C_1()) Error_I2C_1=1;
    if (WriteI2C_1(ControlByte)) Error_I2C_1=4;
    if (IdleI2C_1()) Error_I2C_1=5;
//	ErrorCode = ACKStatus_1();		//Return ACK Status
    if (WriteI2C_1(Address)) Error_I2C_1=4;
    if (IdleI2C_1()) Error_I2C_1=5;
//	ErrorCode = ACKStatus_1();		//Return ACK Status

    if (RestartI2C_1()) Error_I2C_1=2;
    if (WriteI2C_1(ControlByte | 0x01)) Error_I2C_1=4;
    if (IdleI2C_1()) Error_I2C_1=5;
//	ErrorCode = ACKStatus_1();		//Return ACK Status

    if (getsI2C_1(Data, Length)) Error_I2C_1=9;
    if (NotAckI2C_1()) Error_I2C_1=7;
    if (StopI2C_1()) Error_I2C_1=3;
	
    return (Error_I2C_1);
}

/*********************************************************************
* Function:        ACKStatus_1()
* Input:		None.
* Output:		Acknowledge Status.
* Overview:		Return the Acknowledge status on the bus
* Note:			None
********************************************************************/
unsigned int ACKStatus_1(void)
{
    return (!I2C1STATbits.ACKSTAT);		//Return Ack Status
}

/*********************************************************************
* Function:        NotAckI2C_1()
* Input:		None.
* Output:		None.
* Overview:		Generates a NO Acknowledge on the Bus
* Note:			None
********************************************************************/
unsigned int NotAckI2C_1(void)
{
unsigned int time_out;
    I2C1CONbits.ACKDT = 1;			//Set for NotACk
    I2C1CONbits.ACKEN = 1;
    time_out= 0;
    while(I2C1CONbits.ACKEN & time_out<1000)		//wait for ACK to complete
    {
	time_out++;
    }
    I2C1CONbits.ACKDT = 0;			//Set for NotACk
    if(time_out>=1000) return 7;
    return 0;
}

/*********************************************************************
* Function:        AckI2C_1()
* Input:		None.
* Output:		None.
* Overview:		Generates an Acknowledge.
* Note:			None
********************************************************************/
unsigned int AckI2C_1(void)
{
unsigned int time_out;
    I2C1CONbits.ACKDT = 0;			//Set for ACk
    I2C1CONbits.ACKEN = 1;
    time_out= 0;
    while(I2C1CONbits.ACKEN & time_out<1000)		//wait for ACK to complete
    {
    	time_out++;
    }
    if(time_out>=1000) return 9;
    return 0;
}

/*********************************************************************
* Function:       getsI2C_1()
* Input:		array pointer, Length.
* Output:		None.
* Overview:		read Length number of Bytes into array
* Note:			None
********************************************************************/
unsigned int getsI2C_1(unsigned char *rdptr, unsigned char Length)
{
    while (Length --)
    {
    	*rdptr++ = getI2C_1();		//get a single byte
		
//      if(I2C1STATbits.BCL) return(-1);	//Test for Bus collision
	if(I2C1STATbits.BCL) 
	{
		Error_I2C_1=8;		//Test for Bus collision
		I2C1STATbits.BCL= 0;
	}
		
	if(Length)
	{
		AckI2C_1();		//Acknowledge until all read
	}
    }
//  return (Error_I2C_1);
    return(0);
}

/*********************************************************************
* Function:        getI2C_1()
* Input:		None.
* Output:		contents of I2C1 receive buffer.
* Overview:		Read a single byte from Bus
* Note:			None
********************************************************************/
unsigned int getI2C_1(void)
{
    int time_out=0;
    I2C1CONbits.RCEN = 1;		//Enable Master receive
    //Nop();
    while(!I2C1STATbits.RBF & time_out<1000)	//Wait for receive bufer to be full
    {
	time_out++;
    }
    if (time_out>=1000)	Error_I2C_1=9;	//Error_I2C_1=9;
	
    return(I2C1RCV);			//Return data in buffer
}

/*********************************************************************
* Function:        putstringI2C()_1
* Input:		pointer to array.
* Output:		None.
* Overview:		writes a string of data upto PAGESIZE from array
* Note:			None
********************************************************************/
unsigned int putstringI2C_1(unsigned char *wrptr)
{
	unsigned char x;

	for(x = 0; x < PAGESIZE; x++)		//Transmit Data Until Pagesize
	{	
		if(WriteI2C_1(*wrptr))          //Write 1 byte
		{
			return(-3);                        //Return with Write Collision
		}
		IdleI2C_1();                                //Wait for Idle bus
		if(I2C1STATbits.ACKSTAT)
		{
			return(-2);				//Bus responded with Not ACK
		}
		wrptr++;
	}
	return(0);
}

/*********************************************************************
* Function:        EEAckPolling()_1
* Input:		Control byte.
* Output:		error state.
* Overview:		polls the bus for an Acknowledge from device
* Note:			None
********************************************************************/
unsigned int EEAckPolling_1(unsigned char control)
{
    IdleI2C_1();		//wait for bus Idle
    StartI2C_1();		//Generate Start condition
	
    if(I2C1STATbits.BCL)
    {
         return(-1);		//Bus collision, return
    }

    else
    {
         if(WriteI2C_1(control))
        {
            return(-3);	//error return
        }

         IdleI2C_1();		//wait for bus idle
         if(I2C1STATbits.BCL)
         {
             return(-1);	//error return
        }

        while(ACKStatus_1())
        {
            RestartI2C_1();	//generate restart
            if(I2C1STATbits.BCL)
            {
                return(-1);	//error return
            }

            if(WriteI2C_1(control))
            {
             return(-3);
            }

            IdleI2C_1();
        }
    }
     StopI2C_1();			//send stop condition
    if(I2C1STATbits.BCL)
    {
         return(-1);
    }
    return(0);
}

