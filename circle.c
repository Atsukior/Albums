// 在开发板上绘制圆形
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd = open("/dev/fb0", O_RDWR);
    if (fd == -1)
    {
        printf("open error!\n");
        return -1;
    }

    int buf[480][800] = {0};
    int i = 0;
    int j = 0;
    int x = 400;
    int y = 240;
    int r = 100;
    for (i = 0; i < 480;i++)
    {
        for (j = 0; j < 800;j++)
        {
            if ((i - y) * (i - y) + (j - x) * (j - x) <= r * r)
            {
                buf[i][j] = 0xff0000;
            }
        }
    }

    int ret = write(fd, buf, 800 * 480 * 4);
    if (ret == -1)
    {
        printf("write error!\n");
        return -1;
    }

    return 0;
    close(fd);
}