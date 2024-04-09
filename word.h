#ifndef __NUM_H__
#define __NUM_H__


extern unsigned char word[2] [ 16 * 16 / 8];
extern unsigned char int_num[][42];
extern unsigned char Point[];
/*
    @hanzi_display:在开发板任意位置显示汉字
    无返回值
    @const char hanzi[]:要显示的汉字取模后的数据
    @x0,y0:表示从开发板上的哪个位置开始显示该汉字
    @w , h :汉字的宽度和高度
*/
void hanzi_display(const char hanzi[],int x0 , int y0,int w , int h);


void double_display(double digtal, int x0, int y0);

#endif