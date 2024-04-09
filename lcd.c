/*
 * @Date: 2023-12-06 16:10:35
 * @LastEditTime: 2023-12-11 09:44:35
 * @FilePath: \share\lcd.c
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/ioctl.h>

#include "lcd.h"

#define FB_FILE "/dev/fb0"

int fb_fd = -1; // 帧缓冲设备的 文件描述符

int* plcd = NULL; //指向帧缓冲区的首地址

/*
    lcd_open: 打开显示屏设备(帧缓冲设备)
    无参数、无返回值
*/
void lcd_open(void)
{
    int ret;

    //(1) 打开设备
    int fd = open(FB_FILE, O_RDWR);
    if (fd == -1)
    {
        perror("failed to open /dev/fb0");
        return;
    }
    fb_fd = fd;

    // (3) mmap
    plcd = (int *)mmap(NULL, 800 * 480 * 4,
                       PROT_WRITE,
                       MAP_SHARED,
                       fd,
                       0);
    if (plcd == MAP_FAILED)
    {
        perror("failed to mmap");
        close(fd);
        fb_fd = -1;
        return;
    }
}

/*
    lcd_close: 关闭显示屏设备(帧缓冲设备)
    无参数、无返回值
*/
void lcd_close(void)
{
    // (5) 解映射
    munmap(plcd, 800 * 480 * 4);

    //(6) 关闭设备
    close(fb_fd);
}

/*
    lcd_draw_point: 画点函数
    @x: 像素点的x轴坐标
    @y: 像素点的y轴坐标
    @color: 要显示的颜色值
    返回值：
        无。
*/
void lcd_draw_point(int x, int y, int color)
{
    if (x >= 0 && x < 800 && y >= 0 && y < 480)
    {
        *(plcd + y * 800 + x) = color;
    }
}
