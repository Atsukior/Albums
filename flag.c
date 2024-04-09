/*
 * @Date: 2023-12-05 16:40:51
 * @LastEditTime: 2023-12-11 09:52:47
 * @FilePath: \share\flag.c
 */
#include <stdio.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#define WIDTH 800
#define HEIGHT 480

extern int *plcd; // unsigned int *plcd = NULL;
int fd = -1;

/*
Lcd_Init ：LCD初始化，打开LCD屏幕，并完成映射机制
 */
int Lcd_Init()
{
    fd = open("/dev/fb0", O_RDWR);
    if (-1 == fd)
    {
        perror("open error");
        return -1;
    }

    plcd = mmap(NULL, WIDTH * HEIGHT * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (plcd == MAP_FAILED)
    {
        perror("mmap error");
        return -1;
    }

    return 0;
}

/*
Display ：用映射的方法画一个点的颜色
参数：
   color ：画点的颜色
   x ：点的X坐标
   y ：点的Y坐标
 */
void Display(unsigned int color, int x, int y)
{
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
    {
        *(plcd + y * WIDTH + x) = color;
    }
}

/*
Lcd_Uninit ：解映射，关闭LCD屏
 */
void Lcd_Uninit()
{
    munmap(plcd, WIDTH * HEIGHT * 4);
    close(fd);
}

/*
triangle ：画三角形
参数：
    x1 ：三角形第一个顶点的X坐标
    y1 ：三角形第一个顶点的y坐标
    x2 ：三角形第二个顶点的X坐标
    y2 ：三角形第二个顶点的y坐标
    x3 ：三角形第三个顶点的X坐标
    y3 ：三角形第三个顶点的y坐标
    col：三角形的颜色
 */
// 画三角形
void triangle(int x1, int y1, int x2, int y2, int x3, int y3, int col)
{
    // flag:代表本三角形在本直线的左边（0）还是右边（1）（左边右边是抽象概念）
    int i, j, flag1 = 0, flag2 = 0, flag3 = 0;
    float A1, B1, C1, A2, B2, C2, A3, B3, C3;
    // 1号点与2号点的直线方程的A，B，C
    A1 = y2 - y1;
    B1 = x1 - x2;
    C1 = x2 * y1 - x1 * y2;
    // 2号点与3号点的直线方程的A，B，C
    A2 = y2 - y3;
    B2 = x3 - x2;
    C2 = x2 * y3 - x3 * y2;
    // 1号点与3号点的直线方程的A，B，C
    A3 = y3 - y1;
    B3 = x1 - x3;
    C3 = x3 * y1 - x1 * y3;

    // 判断第三个点与直线的相对位置
    if (x3 * A1 + y3 * B1 + C1 > 0)
        flag1 = 1;
    if (x1 * A2 + y1 * B2 + C2 > 0)
        flag2 = 1;
    if (x2 * A3 + y2 * B3 + C3 > 0)
        flag3 = 1;

    for (i = 0; i < 480; i++)
    {
        for (j = 0; j < 800; j++)
        {
            if (flag1 == 1)
            {
                if (flag2 == 1)
                {
                    if (j * A1 + i * B1 + C1 > 0 && j * A2 + i * B2 + C2 > 0 && j * A3 + i * B3 + C3 < 0)
                    {
                        Display(col, i, j);
                    }
                }
                else
                {
                    if (flag3 == 1)
                    {
                        if (j * A1 + i * B1 + C1 > 0 && j * A2 + i * B2 + C2 < 0 && j * A3 + i * B3 + C3 > 0)
                        {
                            Display(col, i, j);
                        }
                    }
                    else
                    {
                        if (j * A1 + i * B1 + C1 > 0 && j * A2 + i * B2 + C2 < 0 && j * A3 + i * B3 + C3 < 0)
                        {
                            Display(col, i, j);
                        }
                    }
                }
            }
            else
            {
                if (flag2 == 0)
                {
                    if (j * A1 + i * B1 + C1 < 0 && j * A2 + i * B2 + C2 < 0 && j * A3 + i * B3 + C3 > 0)
                    {
                        Display(col, i, j);
                    }
                }
                else
                {
                    if (flag3 == 1)
                    {
                        if (j * A1 + i * B1 + C1 < 0 && j * A2 + i * B2 + C2 > 0 && j * A3 + i * B3 + C3 > 0)
                        {
                            Display(col, i, j);
                        }
                    }
                    else
                    {
                        if (j * A1 + i * B1 + C1 < 0 && j * A2 + i * B2 + C2 > 0 && j * A3 + i * B3 + C3 < 0)
                        {
                            Display(col, i, j);
                        }
                    }
                }
            }
        }
    }
}

/*
five_Pointed ：画五角星
参数：
    x ：五角星的在正中心点的X坐标
    y ：五角星的在正中心点的Y坐标
    R ：中心点到外顶点的长度
    col:五角星的颜色
    yDegree ：五角星的倾斜程度
 */
// 画五角星
void five_Pointed(int x, int y, int R, unsigned int col, int yDegree)
{
    struct Vertex
    {
        int x;
        int y;
    };
    struct Vertex RVertex[5], rVertex[5]; // 外围5个顶点的坐标与内部五个顶点的坐标

    double rad = 3.1415926 / 180;                 // 每度的弧度值
    double r = R * sin(18 * rad) / cos(36 * rad); // 五角星短轴的长度
    for (int k = 0; k < 5; k++)                   // 求取坐标
    {
        RVertex[k].x = (int)(x - (R * cos((90 + k * 72 + yDegree) * rad)));
        RVertex[k].y = (int)(y - (R * sin((90 + k * 72 + yDegree) * rad)));
        rVertex[k].x = (int)(x - (r * cos((90 + 36 + k * 72 + yDegree) * rad)));
        rVertex[k].y = (int)(y - (r * sin((90 + 36 + k * 72 + yDegree) * rad)));
    }
    triangle(RVertex[1].x, RVertex[1].y, RVertex[3].x, RVertex[3].y, rVertex[4].x, rVertex[4].y, col);
    triangle(RVertex[2].x, RVertex[2].y, RVertex[4].x, RVertex[4].y, rVertex[0].x, rVertex[0].y, col);
    triangle(RVertex[3].x, RVertex[3].y, RVertex[0].x, RVertex[0].y, rVertex[1].x, rVertex[1].y, col);
}

// 画国旗
void national_flag()
{
    int i = 0, j = 0;
    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            Display(0xff0000, j, i); // 设置像素点颜色为红色
        }
    }
    five_Pointed(120, 120, 72, 0xffff00, 55);
    five_Pointed(48, 240, 24, 0xffff00, 25);
    five_Pointed(96, 288, 24, 0xffff00, 12);
    five_Pointed(168, 288, 24, 0xffff00, 55);
    five_Pointed(216, 240, 24, 0xffff00, 25);
}

// int main()
// {
//     if (Lcd_Init() == -1)
//     {
//         printf("LCD initialize error!\n");
//         return -1;
//     }

//     national_flag();

//     Lcd_Uninit();

//     return 0;
// }
