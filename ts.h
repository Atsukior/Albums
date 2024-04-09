/*
 * @Date: 2023-12-08 15:17:56
 * @LastEditTime: 2023-12-08 15:19:00
 * @FilePath: \share\ts.h
 */
#ifndef __TS_H__
#define __TS_H__

typedef struct point
{
    int x; // 点击“点”的x轴坐标
    int y; // 点击“点”的y轴坐标

} ts_point;

// “滑动方向”
typedef enum
{
    MOVE_UP = 1, // 向上滑动
    MOVE_DOWN,   // 向下滑动
    MOVE_LEFT,   // 向左滑动
    MOVE_RIGHT,  // 向右滑动

    MOVE_UNKNOWN = 100
} move_dir_t;

// 获取手指在触摸屏上的滑动方向
move_dir_t get_ts_direction(void);

// 获取触摸屏点击事件
void get_ts_point(ts_point *);

#endif
