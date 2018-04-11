/* 
 * File:   CAD_LAVB_G6_v7.h (LAVB: Last Analog Values Buffer)
 * Author: Christian Mares & Imanola atienza
 *
 * Descripcion: Este modulo sirve para guardar los ultimos 8 valores leidos por
 *              el CAD con la intencion de poder obetener la media de estos.
 *
 * Created on 30 de marzo de 2016, 15:23
 */

// Metodos de Modificacion
void add_new(int, int);
void add_potenc_new(int);
void add_termom_new(int);

// Metodos de lectura
int get_media(int pata);
int get_last(int pata);
int get_any(int pata, int val);
int get_potenc_media();
int get_potenc_last();
int get_potenc_any(int val);
int get_termom_media();
int get_termom_last();
int get_termom_any(int);
int get_joy_minValX();
int get_joy_maxValX();
int get_joy_cenValX();
int get_joy_minValY();
int get_joy_maxValY();
int get_joy_cenValY();
void set_minY(int);
void set_minX(int);
void set_maxY(int);
void set_maxX(int);
void calibrar(int,int);