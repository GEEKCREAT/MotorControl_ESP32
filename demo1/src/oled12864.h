#ifndef OLED12864_H
#define OLED12864_H

#include "Arduino.h"
#include "interface.h"
#include "fonts.h"
#define OLED_ADDRESS 0x3C
#define PI 3.141592654
#define X_WIDTH 128
#define Y_WIDTH 64
#define Y_WIDTH_ 8

void OLED_Init(void);
void OLED_Cache_to_hardware();

void OLED_Setting_luminance(unsigned char a);

void OLED_set_dot(unsigned char x,unsigned char y,unsigned char dot_type);
void OLED_Pix(unsigned char x,unsigned char y,unsigned char w,unsigned char h,const char *p,unsigned char type);

void OLED_Ascii(unsigned char x,unsigned char y,unsigned char size,char *chr,unsigned char type);
void OLED_Ch(unsigned char x,unsigned char y,unsigned char size,char *chr,unsigned char type);
void OLED_Str(unsigned char x,unsigned char y,unsigned char size,char *str,unsigned char lenconst,unsigned char type);
void OLED_Str_list(unsigned char x,unsigned char y,unsigned char size,char *str,unsigned char lenstard,unsigned char lenconst,unsigned char type);

void OLED_AL(int sx,int sy,int ex,int ey,unsigned char dot_type);

void OLED_square(int Start_x, int Start_y, int End_x, int End_y, char type);

#endif