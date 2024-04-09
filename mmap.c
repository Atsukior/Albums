#include<stdio.h>
#include<sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int *plcd = NULL;

void draw(int x,int y,int color)
{
    if (x >= 0 && x < 800 && y >= 0 && y < 480)
    {
        *(plcd + 800 * y + x) = color;
    }
}

int main()
{
    fd = open("/dev/fb0", O_RDWR);
    if (fd == -1)
    {
        printf("open error!\n");
        return -1;
    }

    plcd = mmap(NULL, 480 * 800 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (plcd == MAP_FAILED)
    {
        printf("mmap error!\n");
        return -1;
    }

    int i, j;
    int color = 0xff0000;
    for (i = 0; i < 800;i++)
    {
        for (j = 0;j<480,j++)
        {
            draw(i, j, color);
        }
    }

    munmap(plcd, 480 * 800 * 4);

    close(fd);
    return 0;
}