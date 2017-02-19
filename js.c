#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <wiringPi.h>
#include <linux/joystick.h>

#define SD 23
#define IN_R 24
#define IN_L 25

int main(void)
{
    int fd;
    int num;
    unsigned char axes = 2;
    unsigned char buttons = 2;
    int version = 0x000800;
    char name[128] = "Unknown";
    struct JS_DATA_TYPE js;
    
    pinMode(SD,OUTPUT);
    pinMode(IN_R,OUTPUT);
    pinMode(IN_L,OUTPUT);

    digitalWrite(SD,1);

    fd = open("/dev/input/js0",O_RDONLY);
    if(fd < 0)
    {
        puts("ERROR");
        exit(1);
    }
    ioctl(fd, JSIOCGVERSION, &version);
    ioctl(fd, JSIOCGAXES, &axes);
    ioctl(fd, JSIOCGBUTTONS, &buttons);
    ioctl(fd, JSIOCGNAME(128), name);
             
    printf("Joystick (%s) has %d axes and %d buttons. Driver version is %d.%d.%d.\n",
            name, axes, buttons, version >> 16, (version >> 8) & 0xff, version & 0xff);
    printf("Testing ... (interrupt to exit)\n");

    while(1){
        read(fd, &js, JS_RETURN);
        num = js.y;
        if(num == 255){
            puts("F95%");
        }else if(num > 255){
            puts("F90%");
        }else if(num > 247){
            puts("F85%");
        else if(num > 128){
            puts("5%");
        }else if(num == 128){
            puts("0%");
        }else if(num > 121){
            puts("5%");
        }else if(num > 114){
            puts("10%");
        }else if(num > 107){
            puts("15%");
        }else if(num > 100){
            puts("20%");
        }else if(num > 93){
            puts("25%");
        }else if(num > 86){
            puts("30%");
        }else if(num > 79){
            puts("35%");
        }else if(num > 72){
            puts("40%");
        }else if(num > 65){
            puts("45%");
        }else if(num > 58){
            puts("50%");
        }else if(num > 51){
            puts("55%");
        }else if(num > 44){
            puts("60%");
        }else if(num > 37){
            puts("65%");
        }else if(num > 30){
            puts("70%");
        }else if(num > 23){
            puts("75%");
        }else if(num > 16){
            puts("80%");
        }else if(num > 9){
            puts("85%");
        }else if(num > 1){
            puts("90%");
        }else if(num == 1){
            puts("95%");
        }else {
            puts("other");
        }
        //printf("%d\n",js.y);
        usleep(10000);
    }
    return 0;
}

