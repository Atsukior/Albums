// 从键盘输入一个数字（小数点后两位），显示在开发板上。
#include "lcd.h"
#include "num.h"
#include <stdio.h>
#include <string.h>

void display_number(float num)
{
    // 将浮点数转换为字符串
    char str[10];
    sprintf(str, "%.2f", num);

    // 遍历字符串中的每个字符
    for (int i = 0; i < strlen(str); i++)
    {
        // 根据字符选择相应的代码数组
        switch (str[i])
        {
        case '0':
            // 显示数字0的代码
            return CODE_0;
            break;
        case '1':
            // 显示数字1的代码
            CODE_1;
            break;
        case '2':
            // 显示数字2的代码
            CODE_2;
            break;
        case '3':
            // 显示数字3的代码
            CODE_3;
            break;
        case '4':
            // 显示数字4的代码
            CODE_4;
            break;
        case '5':
            // 显示数字5的代码
            CODE_5;
            break;
        case '6':
            // 显示数字6的代码
            CODE_6;
            break;
        case '7':
            // 显示数字7的代码
            CODE_7;
            break;
        case '8':
            // 显示数字8的代码
            CODE_8;
            break;
        case '9':
            // 显示数字9的代码
            CODE_9;
            break;
        case '.':
            // 显示小数点的代码
            CODE_DOT;
            break;
        }
    }
}

void num_display(const char num[], int x0, int y0, int w, int h)
{
    int dian; // 第dian个像素点

    for (dian = 0; dian < w * h; dian++)
    {
        // 要在屏幕上的合适位置显示这个像素点

        int x, y; // （x,y)这个dian像素点，在屏幕上的显示位置

        y = y0 + dian / w; // 该点在原始汉字的第dian / w行

        x = x0 + dian % w; // 该点在原始汉字的第dian % w列

        // dian这个像素点取模后在数组hanzi[]的第几个元素中呢？
        int index = dian / 8; // 表示数组元素的下标

        // hanzi[index] 包括8个bit位，那么该点到底在第几bit位呢？
        int bit = 7 - dian % 8; // 在字节中的第几bits

        int color; // 这个像素点要显示的颜色

        // 如果该bits为1表示显示颜色，为0表示不显示颜色
        if (num[index] & (1 << bit))
        {
            // 该像素点在笔划上
            color = 0;
        }
        else
        {
            // 该像素点不在笔划上
            color = 0xff0000;
        }

        lcd_draw_point(x, y, color);
    }
}