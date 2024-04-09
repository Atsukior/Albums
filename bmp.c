
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "lcd.h" //需要调用lcd.h中的函数

/*
    bmp_display: 在屏幕的坐标(x0,y0)处显示一张指定的bmp图片
    @bmp_file: 要显示的bmp图片的文件名
    @x0: 显示位置左上顶点的x轴坐标
    @y0: 显示位置左上顶点的y轴坐标
    返回值：
        无。
*/
// const char *bmp_files[] = {"/sarff/image1", "/sarff/genshin", "/sarff/image2", "/sarff/image3"};

void bmp_display(const char *bmp_files[], int x0, int y0)
{
    int fd;

    fd = open(bmp_files, O_RDONLY);
    if (fd == -1)
    {
        perror("failed to open bmp_file");
        return;
    } // 判断是否打开bmp图片

    int width, height;
    short depth;
    unsigned char buf[4];

    lseek(fd, 0x12, SEEK_SET);                                        // 把光标移动到0x12的位置(宽度)
    read(fd, buf, 4);                                                 // 读取0x12~0x15的数据保存至buf里面
    width = (buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) | buf[0]; // 转化成我们需要的格式 //  1 2 3 4 --->4 3 2 1

    lseek(fd, 0x16, SEEK_SET); // 把光标移到0x16的位置（高度）
    read(fd, buf, 4);          // 读取0x16~0x19的数据保存至buf里面
    height = (buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) | buf[0];

    lseek(fd, 0x1C, SEEK_SET); // 把光标移动到0x1c的位置（色深）
    read(fd, buf, 2);          // 读取0x1c和0x1d的数据保存在buf里面
    depth = (buf[1] << 8) | buf[0];

    printf("%d x %d\n", width, height); // 输出我们的宽度和高度大小

    if (!(depth == 24 || depth == 32))
    {
        printf("Sorry, Not Supported Bmp Format!\n");
        close(fd);

        return;
    } // 判断是否为bmp图片

    int valid_bytes_per_line; // 每一行有效的数据字节数
    int laizi = 0;            // 每一行末尾的填充的“赖子”数
    int total_bytes_per_line; // 每一行实际的字节数.
    int total_bytes;          // 整个像素数组的字节数

    valid_bytes_per_line = abs(width) * (depth / 8); // 求每一行有效的数据字节数 ， abs是用来求绝对值

    if (valid_bytes_per_line % 4)
    {
        laizi = 4 - valid_bytes_per_line % 4; // 求每一行末尾的填充的“赖子”数
    }

    total_bytes_per_line = valid_bytes_per_line + laizi; // 求每一行实际的字节数

    total_bytes = abs(height) * total_bytes_per_line; // 求整个像素数组的字节数

    unsigned char pixel[total_bytes]; // 用来保存像素数组数据的一个数组（开辟一快内存来保存读取的数据）
    lseek(fd, 54, SEEK_SET);          // 把光标移动到像素数组的位置
    read(fd, pixel, total_bytes);     // 把像素数组的数据读取出来存放在数组里

    // 解析像素数据，并在屏幕上显示
    unsigned char a, r, g, b;
    int color;
    int i = 0;
    int x, y;

    for (y = 0; y < abs(height); y++)
    {
        for (x = 0; x < abs(width); x++)
        {
            b = pixel[i++];
            g = pixel[i++];
            r = pixel[i++];
            if (depth == 32) // 判断是32位bmp，是32位的有a(透明度)
            {
                a = pixel[i++];
            }
            else
            {
                a = 0;
            }
            color = (a << 24) | (r << 16) | (g << 8) | b; // 转换成我们需要的格式

            int x1, y1; // 该像素点在屏幕上显示的  坐标

            x1 = (width > 0) ? (x0 + x) : (x0 + abs(width) - 1 - x);
            y1 = (height > 0) ? (y0 + height - 1 - y) : y0 + y;
            lcd_draw_point(x1, y1, color);
        }

        i += laizi; // 跳过“赖子”  //赖子是无效数据，需要跳过 i = i + laizi;
    }

    close(fd);
}
