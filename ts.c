#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include<stdlib.h>
#include <linux/input.h>


#include "ts.h"
//获取触摸屏点击事件
void get_ts_point(ts_point* p)
{
    int ret;
    int fd;

    fd = open("/dev/input/event0", O_RDONLY); //打开触摸事件对应的文件
    if (fd == -1)
    {
        perror("failed to open /dev/input/event0");
        return ;
    }

    int x1 = -1, y1 = -1; //记录点击事件中第一个点的坐标
    int x2, y2; //记录点出事件中最后一个点的坐标

    while (1)
    {
        struct input_event ev; //定义了一个输入事件结构体
        ret = read(fd, &ev, sizeof(ev)); //通过read 函数读取该结构体
        if (ret != sizeof(ev))  //如果读取的数据不等于结构体的大小，说明没有读完，就继续把该结构体读完
        {
            continue;
        }

        // printf("type: %x  code: %x  value: %x\n", ev.type, ev.code, ev.value);

        //触摸屏的x轴事件
        if (ev.type == EV_ABS && ev.code == ABS_X)
        {
            if (x1 == -1) //第一次按下值赋值给第一个的坐标
            {
                x1 = ev.value;
            }
            x2 = ev.value; //后续按下的值赋值第二个的坐标
        }


        
        //触摸屏的y轴事件
        if (ev.type == EV_ABS && ev.code == ABS_Y)
        {
            if (y1 == -1)
            {
                y1 = ev.value;
            }
            y2 = ev.value;
        }

        //触摸屏弹起事件  //
        if ( (ev.type == EV_KEY && ev.code == BTN_TOUCH &&  ev.value == 0) ||
             (ev.type == EV_ABS && ev.code == ABS_PRESSURE && ev.value ==0) )
        {
            x1 = x1 * 800.0 / 1024; //新版的开发板的坐标系时1024 x 600
            x2 = x2 * 800.0 / 1024;

            y1 = y1 * 480.0 / 600;
            y2 = y2 * 480.0 / 600;

            p->x = x2;
            p->y = y2;
            break;
        }
    }


    close(fd);
}

//获取手指在触摸屏上的滑动方向
move_dir_t get_ts_direction(void)
{
    int ret;
    int fd;
    move_dir_t dir = MOVE_UNKNOWN;

    fd = open("/dev/input/event0", O_RDONLY);
    if (fd == -1)
    {
        perror("failed to open /dev/input/event0");
        return -1;
    }

    int x1 = -1, y1 = -1; //记录点击事件中第一个点的坐标
    int x2, y2; //记录点出事件中最后一个点的坐标

    while (1)
    {
        struct input_event ev;
        ret = read(fd, &ev, sizeof(ev));
        if (ret != sizeof(ev))
        {
            continue;
        }

        // printf("type: %x  code: %x  value: %x\n", ev.type, ev.code, ev.value);

        //触摸屏的x轴事件
        if (ev.type == EV_ABS && ev.code == ABS_X)
        {
            if (x1 == -1)
            {
                x1 = ev.value;
            }
            x2 = ev.value;
        }


        
        //触摸屏的y轴事件
        if (ev.type == EV_ABS && ev.code == ABS_Y)
        {
            if (y1 == -1)
            {
                y1 = ev.value;
            }
            y2 = ev.value;
        }
        //触摸屏弹起事件
        if ( (ev.type == EV_KEY && ev.code == BTN_TOUCH &&  ev.value == 0) ||
             (ev.type == EV_ABS && ev.code == ABS_PRESSURE && ev.value ==0) )
        {
            int delt_x = abs(x2 - x1);
            int delt_y = abs(y2 - y1);

            if (delt_x >= 2 * delt_y)
            {
                if (x2 > x1)
                {
                    dir = MOVE_RIGHT;
                }
                else
                {
                    dir = MOVE_LEFT;
                }

                break;
            }
            else if (delt_y >= 2 * delt_x)
            {
                if (y2 > y1)
                {
                    dir = MOVE_DOWN;
                }
                else
                {
                    dir = MOVE_UP;
                }
                break;
            }
            else 
            {
                //方向不明，请继续
                x1 = -1;
                y1 = -1;
            }
        }
    }


    close(fd);

    return dir;
}