#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <softPwm.h>

#include <wiringPi.h>
#include <linux/joystick.h>

#define SD 23
#define R 24
#define L 25
#define RANGE 100

int main(void)
{
    int fd;
    int flag = 0;
    float num,rate,percent;
    unsigned char axes = 2;
    unsigned char buttons = 2;
    int version = 0x000800;
    char name[128] = "Unknown";
    struct JS_DATA_TYPE js;
   
    wiringPiSetup();

    pinMode(SD,OUTPUT);
    pinMode(R,OUTPUT);
    pinMode(L,OUTPUT);

    digitalWrite(SD,HIGH);

    softPwmCreate(R,0,RANGE);
    softPwmCreate(L,0,RANGE);

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
        num = num - 128;
        if(num < 0){
            num = num * -1;
            flag = 1;
        }else {
            flag = 0;
        }
        rate = num / 127;
        percent = rate * 95;
        printf("%.0f\n",percent);
        if(percent != 0){
        }else {
            softPwmWrite(R,0);
            softPwmWrite(L,0);
        }
        if(flag == 1){
            softPwmWrite(R,percent);
            delay(3);
        }else {
            softPwmWrite(L,percent);
            delay(3);
        }
        usleep(10000);
    }
    return 0;
}

