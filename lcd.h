
#ifndef __LCD_H__
#define __LCD_H__



/*
    lcd_open: 打开显示屏设备(帧缓冲设备)
    无参数、无返回值
*/
void lcd_open(void);

/*
    lcd_close: 关闭显示屏设备(帧缓冲设备)
    无参数、无返回值
*/
void lcd_close(void);

/*
    lcd_draw_point: 画点函数
    @x: 像素点的x轴坐标
    @y: 像素点的y轴坐标
    @color: 要显示的颜色值
    返回值：
        无。
*/
void lcd_draw_point(int x, int y, int color);

/*
    lcd_draw_rectangle：在屏幕上画一个矩形
    @x0: 矩形的左上顶点的x轴坐标
    @y0: 矩形的左上顶点的y轴坐标
    @w: 矩形的宽度
    @h: 矩形的高度
    @color: 矩形的填充颜色
    返回值：
        无
*/

#endif 