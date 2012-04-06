#include "simulate.h"

int main()
{
    int fd_kbd; 
    int fd_mouse;
    int i;

	//How to get the device automatically is a problem........
    fd_kbd = open("/dev/input/event4",O_RDWR);
    if(fd_kbd < 0){
        printf("error open keyboard:\n");
        return -1;
    }

    fd_mouse = open("/dev/input/event5",O_RDWR); 
    //fd_mouse = open("/dev/input/mice",O_RDWR);???
    if(fd_mouse < 0){
        printf("error open mouse\n");
        return -2;
    }

    for(i=0; i<10; i++)
    {
        simulate_key(fd_kbd, 1, KEY_A + i); //press down
        simulate_key(fd_kbd, 0, KEY_A + i); //release
        simulate_mouse(fd_mouse, REL_X, i*10);
        simulate_mouse(fd_mouse, REL_Y, i*10);
        sleep(1);
    }

    close(fd_kbd);
    close(fd_mouse);
    return 0;
}

