#include "i2c_Func_DSE.h"
#include "SRF08_G6_v7.h"
#include "LCD_Explorer16_24H_G6_v7.h"

unsigned char dirSensor = 0xE2;
unsigned char dat[3];
char read = 0;
int the_niggers_dick;

void Inic_SFR08(){
    Inic_Timer_5();
    inic_measure();
}
void Inic_Timer_5 (void){
// Init Timer
 TMR5 = 0; // Borra el timer
 //PR1 = 40000-1; // Configura el periodo -> Mhz x 1000ms - 1
 PR5 = 15625-1; // 100ms

// T1CON = 0x8000; // enabled, prescaler 1:1, internal clock
// T1CON = 0b1000000000000000; // enabled, prescaler 1:1, internal clock
 T5CON = 0;
 T5CONbits.TCKPS = 3; // VAlor del prescaler
 //_T1CONbits.TON; // Pone en marcha el Timer 1
// _T1IP = 4; // prioridad por defecto
// IPC0bits.T1IP = 4; // Equivale a _T1IP
 _T5IF = 0; // Borra flag de interrupción
 //IFS0bits.T1IF = 0; // Equivale a _T1IF
 _T5IE = 1; // Habilita interrupción del Timer 1
 // IEC0bits.T1IE = 1; // Equivale a _T1IE
T5CONbits.TON =1; // Pone en marcha el Timer 1
} // Fin Inic_Cronometro

void inic_measure() {
   LDByteWriteI2C_1(dirSensor, 0, 0x51); // El 1º parametro es la dir del sensor, el 2º la ubicacion del registro y el 3º el dato
}
void read_measure_sensor(void) {
    LDByteReadI2C_1(dirSensor, 1, &dat[0], 3);
}
void change_dirI2C(unsigned char old_dir, unsigned char new_dir) {
    LDByteWriteI2C_1(old_dir, 0, 0XA0);
    LDByteWriteI2C_1(old_dir, 0, 0XAA);
    LDByteWriteI2C_1(old_dir, 0, 0XA5);
    LDByteWriteI2C_1(old_dir, 0, new_dir);

    dirSensor = new_dir;
}
void read_dirI2C(unsigned char check_dir){
    unsigned int ACKStatus;
    if (IdleI2C_1())
        ACKStatus=5;
    if (StartI2C_1())
        ACKStatus=2;
    if (WriteI2C_1(check_dir))
        ACKStatus=4;
    if (IdleI2C_1())
        ACKStatus=5;
    unsigned int time_out=0;
    while (I2C1STATbits.ACKSTAT && time_out<2000)		//Wait for bus Idle
    {
        time_out++;
    }
    ACKStatus = (time_out-1000)? 1:0;
    if (StopI2C_1())
        ACKStatus=3;
    switch (ACKStatus){
        case 0:
            set_string_ventana("No ACK",7,9);
            break;
        case 1:
            set_string_ventana(" ACK  ",7,9);
            break;
        case 2:
            set_string_ventana("Error at Start ",7,0);
            break;
        case 3:
            set_string_ventana("Error at Stop  ",7,0);
            break;
        case 4:
            set_string_ventana("Error at check ",7,0);
            break;
        case 5:
            set_string_ventana("Error: Idle    ",7,0);
            break;
    }
}
char get_sensor_data(int i) {
    return dat[i];
}
int get_distance() {
    the_niggers_dick = dat[1];
    the_niggers_dick = (the_niggers_dick << 8) | dat[2];
    return the_niggers_dick;
}
char i2c_read(){
    return read;
}
void i2c_set_read(char v){
    read=v;
}
void _ISR _T5Interrupt(void){

    read_measure_sensor();
    inic_measure();
    the_niggers_dick = dat[1];
    the_niggers_dick = (the_niggers_dick << 8) | dat[2];
//    set_int_ventana(the_niggers_dick,7,10);

    _T5IF = 0;
}