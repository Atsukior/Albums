/*
 * @Date: 2023-12-08 09:05:47
 * @LastEditTime: 2023-12-08 13:32:14
 * @FilePath: \share\bmp.h
 */
#ifndef __BMP_H__
#define __BMP_H__

/*
    bmp_display: 在屏幕的坐标(x0,y0)处显示一张指定的bmp图片
    @bmp_file: 要显示的bmp图片的文件名
    @x0: 显示位置左上顶点的x轴坐标
    @y0: 显示位置左上顶点的y轴坐标
    返回值：
        无。
*/
void bmp_display(const char *bmp_files[], int x0, int y0);

#endif