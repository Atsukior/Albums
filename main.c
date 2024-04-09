/*
 * @Date: 2023-12-07 10:23:09
 * @LastEditTime: 2023-12-11 12:19:23
 * @FilePath: \share\main.c
 */
// #include <stdio.h>
// #include "lcd.h"
// #include "word.h"

// int main()
// {
//     lcd_open();

//     hanzi_display(word[0], 100, 240, 24, 46);
//     hanzi_display(word[1], 200, 240, 24, 46);
//     hanzi_display(word[2], 300, 240, 24, 46);
//     hanzi_display(word[3], 400, 240, 24, 46);
//     hanzi_display(word[4], 500, 240, 24, 46);
//     hanzi_display(word[5], 600, 240, 24, 46);
//     hanzi_display(word[6], 700, 240, 24, 46);

//     lcd_close();
// }

// #include "num.h"
// #include "lcd.h"
// #include <stdio.h>

// int main()
// {
//     lcd_open();

//     // 从键盘输入一个数字
//     double num;
//     printf("please input a number:");
//     scanf("%lf", &num);

//     // 调用double_display函数来显示数字
//     double_display(num, 0, 0);

//     lcd_close();

//     return 0;
// }

// 循环播放图片
//  #include "bmp.h"
//  #include "lcd.h"
//  #include <stdio.h>
//  #include <unistd.h>

// const char *bmp_files[] = {"/sarff/image1", "/sarff/genshin", "/sarff/image2", "/sarff/image3"};

// int main()
// {
//     lcd_open();

//     int num_images = 4;
//     int i;
//     while(1)
//     {
//         bmp_display(bmp_files[i], 0, 0);
//         usleep(1000000); // 延时1秒钟
//         i++;
//         if (i == 4)
//         {
//             i = 0;
//         }
//     }

//     lcd_close();

//     return 0;
// }

// 触摸滑动切换图片
//  #include <stdio.h>
//  #include "lcd.h"
//  #include "word.h"
//  #include "bmp.h"
//  #include "ts.h"

// const char *bmp_files[] = {"/sarff/image1", "/sarff/mihoyo", "/sarff/genshin", "/sarff/image2", "/sarff/image3", "/sarff/image4", "/sarff/image5"};
// int current_image = 0;
// int num_images = sizeof(bmp_files) / sizeof(bmp_files[0]);

// int main()
// {
//     lcd_open();

//     //    hanzi_display(word[0],100,100,16,16);
//     //    hanzi_display(word[1],130,100,16,16);
//     // ts_point *p = (ts_point*)malloc(sizeof(ts_point));
//     // get_ts_point(p);

//     // get_ts_direction();

//     while (1)
//     {
//         ts_point point;
//         get_ts_point(&point); // 获取触摸屏点击事件

//         move_dir_t direction = get_ts_direction(); // 获取手指在触摸屏上的滑动方向

//         if (direction == MOVE_UP)
//         {
//             current_image++; // 切换到下一张图片
//             if (current_image >= num_images)
//             {
//                 current_image = 0; // 如果已经是最后一张图片，则切换到第一张图片
//             }
//         }
//         else if (direction == MOVE_DOWN)
//         {
//             current_image--; // 切换到上一张图片
//             if (current_image < 0)
//             {
//                 current_image = num_images - 1; // 如果已经是第一张图片，则切换到最后一张图片
//             }
//         }
//         else if (direction == MOVE_LEFT)
//         {
//             current_image++; // 切换到下一张图片
//             if (current_image >= num_images)
//             {
//                 current_image = 0; // 如果已经是最后一张图片，则切换到第一张图片
//             }
//         }
//         else if (direction == MOVE_RIGHT)
//         {
//             current_image--; // 切换到上一张图片
//             if (current_image < 0)
//             {
//                 current_image = num_images - 1; // 如果已经是第一张图片，则切换到最后一张图片
//             }
//         }

//         // 根据当前图片索引显示图片
//         bmp_display(bmp_files[current_image], 0, 0);
//     }

//     lcd_close();
// }

// 循环播放时随意退出，不能终止进程，只退出循环
#include "ts.h"
#include "bmp.h"
#include "lcd.h"
#include "flag.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

const char *bmp_files[] = {"/sarff/image1", "/sarff/mihoyo", "/sarff/genshin", "/sarff/image2", "/sarff/image3", "/sarff/image4", "/sarff/image5"};
int current_image = 0;
int num_images = sizeof(bmp_files) / sizeof(bmp_files[0]);

// #define WIDTH 800
// #define HEIGHT 480

// unsigned int *plcd = NULL;
// int fd = -1;

// 创建一个函数，调用ts.c中的get_ts_point，来判断是否按下开发板100*100的区域，即click是否为1
int flag = 114514;

void *click()
{
    ts_point *p = (ts_point *)malloc(sizeof(ts_point));
    while (1)
    {
        get_ts_point(p); // 获取触摸屏点击事件

        // 判断是否点击了开发板左上角100*100的区域，如果是，则设置 flag 为 0 暂停循环
        if (p->x >= 0 && p->x <= 100 && p->y >= 0 && p->y <= 100)
        {
            flag = 0;
        }
        // 判断是否点击了开发板右下角100*100的区域，如果是，则设置 flag 为 1 继续循环
        else if (p->x >= 700 && p->x <= 800 && p->y >= 380 && p->y <= 480)
        {
            flag = 1;
        }
    }
}

int main()
{
    lcd_open();

    int num_images = 7;
    // int i;

    pthread_t tid1;
    pthread_create(&tid1, NULL, (void *)click, NULL);

    if (Lcd_Init() == -1)
    {
        printf("LCD initialize error!\n");
        return -1;
    }

    national_flag();

    // Lcd_Uninit();
    while (1)
    {
        if (flag == 1)
        {

            bmp_display(bmp_files[current_image], 0, 0);
            usleep(1000000); // 延时1秒钟
            current_image++;
            if (current_image == 7)
            {
                current_image = 0;
            }
        }
        else
        {
            ts_point point;
            get_ts_point(&point); // 获取触摸屏点击事件

            move_dir_t direction = get_ts_direction(); // 获取手指在触摸屏上的滑动方向

            if (direction == MOVE_UP)
            {
                current_image++; // 切换到下一张图片
                if (current_image >= num_images)
                {
                    current_image = 0; // 如果已经是最后一张图片，则切换到第一张图片
                }
            }
            else if (direction == MOVE_DOWN)
            {
                current_image--; // 切换到上一张图片
                if (current_image < 0)
                {
                    current_image = num_images - 1; // 如果已经是第一张图片，则切换到最后一张图片
                }
            }
            else if (direction == MOVE_LEFT)
            {
                current_image++; // 切换到下一张图片
                if (current_image >= num_images)
                {
                    current_image = 0; // 如果已经是最后一张图片，则切换到第一张图片
                }
            }
            else if (direction == MOVE_RIGHT)
            {
                current_image--; // 切换到上一张图片
                if (current_image < 0)
                {
                    current_image = num_images - 1; // 如果已经是第一张图片，则切换到最后一张图片
                }
            }

            // 根据当前图片索引显示图片
            bmp_display(bmp_files[current_image], 0, 0);
        }
    }

    lcd_close();

    return 0;
}