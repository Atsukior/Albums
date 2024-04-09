/*
 * @Date: 2023-12-11 09:20:01
 * @LastEditTime: 2023-12-11 09:23:06
 * @FilePath: \share\flag.h
 */
#ifndef __FLAG_H__
#define __FLAG_H__

void Display(unsigned int color, int x, int y);
void Lcd_Uninit();
void triangle(int x1, int y1, int x2, int y2, int x3, int y3, int col);
void five_Pointed(int x, int y, int R, unsigned int col, int yDegree);
void national_flag();

#endif
