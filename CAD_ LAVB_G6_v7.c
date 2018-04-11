/*
 * File:   CAD_LAVB_G6_v7.h (LAVB: Last Analog Values Buffer)
 * Author: Christian Mares & Imanol atienza
 *
 * Created on 30 de marzo de 2016, 15:23
 */

int LAVB[16][8];
int last_pos_array[16];
int minX, maxX, cenVX;
int minY, maxY, cenVY;



//============================ FUNCIONES GENERALES =============================
void add_new(int pata, int nVal){
    if(pata>=0 && pata<16){
        LAVB[pata][last_pos_array[pata]] = nVal;
        if(last_pos_array[pata]<7)
            last_pos_array[pata]++;
        else
            last_pos_array[pata]=0;
    }
}
void add_potenc_new(int nVal){
    LAVB[5][last_pos_array[5]] = nVal;
    if(last_pos_array[5]<7)
        last_pos_array[5]++;
    else
        last_pos_array[5]=0;
}
void add_termom_new(int nVal){
    LAVB[4][last_pos_array[4]] = nVal;
    if(last_pos_array[4]<7)
        last_pos_array[4]++;
    else
        last_pos_array[4]=0;
}

int get_media(int pata){
    //if(pata>=0 && pata<16)
        return (LAVB[pata][0]+
                LAVB[pata][1]+
                LAVB[pata][2]+
                LAVB[pata][3]+
                LAVB[pata][4]+
                LAVB[pata][5]+
                LAVB[pata][6]+
                LAVB[pata][7])>>3;
//    else
//        return -1;
}
int get_last(int pata){
//    if(pata>=0 && pata<16)
        return LAVB[pata][last_pos_array[pata]];
//    else
//        return -1;
}
int get_any(int pata, int val){
//    if(pata>=0 && pata<16 && val>=0 && val <8)
        return LAVB[pata][val];
//    else
//        return -1;
}
int get_potenc_media(){
    return get_media(5);
}
int get_potenc_last(){
    return get_last(5);
}
int get_potenc_any(int val){
//    if(val>=0 && val <8)
        return get_any(5,val);
//    else
//        return -1;
}
int get_termom_media(){
    return get_media(4);
}
int get_termom_last(){
    return get_last(4);
}
int get_termom_any(int val){
//    if(val>=0 && val <8)
        return get_any(4,val);
//    else
//        return -1;
}

//=========================== FUNCIONES DE CALIBRADO ===========================
void calibrar(int chosa,int nVal) {
    if(chosa==8){
        if(nVal>maxX) maxX=nVal;
        else if(nVal<minX) minX=nVal;
        cenVX=(maxX-minX)/2;
    }
    else if(chosa==9){
        if(nVal>maxY) maxY=nVal;
        else if(nVal<minY) minY=nVal;
        cenVY=(maxY-minY)/2;
    }
}

void set_maxX(int nVal) {
    if(nVal > maxX){
        maxX = nVal;
        cenVX = (minX-maxX)>>1;
    }
}
void set_minX(int nVal) {
    if(nVal < minX){
        minX = nVal;
        cenVX = (minX-maxX)>>1;
    }
}
void set_maxY(int nVal) {
    if(nVal > maxY){
        maxY = nVal;
        cenVX = (minY-maxY)>>1;
    }
}
void set_minY(int nVal) {
    if(nVal < minY){
        minY = nVal;
        cenVX = (minY-maxY)>>1;
    }
}

int get_joy_minValX() {
    return minX;
}
int get_joy_maxValX() {
    return maxX;
}
int get_joy_cenValX() {
    return cenVX;
}

int get_joy_minValY() {
    return minY;
}
int get_joy_maxValY() {
    return maxY;
}
int get_joy_cenValY() {
    return cenVY;
}