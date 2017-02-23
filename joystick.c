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

#define SD 23   //33番ピン
#define R 24    //35番ピン
#define L 25    //37番ピン
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
   
    wiringPiSetup();    //wiringPi初期化

    pinMode(SD,OUTPUT);     //33,35,37番ピンを出力に設定
    pinMode(R,OUTPUT);  
    pinMode(L,OUTPUT);

    digitalWrite(SD,HIGH);  //33番ピンから3.3Vを出力

    softPwmCreate(R,0,RANGE);   //35,37番ピンをソフトpwmに設定する
    softPwmCreate(L,0,RANGE);   //初期値0、最大値100
    
    /*-----ここからコントローラの情報を取得する初期設定-----*/
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

    /*-----ここまで-----*/

    while(1){       //無限ループ
        read(fd, &js, JS_RETURN);   //左ジョイスティックの値を取得
        num = js.y;         //ジョイスティックの縦方向の大きさをnumに代入
        //ここから取得値を必要な値に調整する
        //numの値:1〜255(何も動かさない状態を50)
        num = num - 128;    //numから128を引く(-127〜127)
        if(num < 0){        //numが負なら-1をかけ、正にする
            num = num * -1;     //num(127〜127)
            flag = 1;   //負のときflagを1
        }else {
            flag = 0;   //正ならflagを0
        }
        rate = num / 127;   //rateにnumを127割った数を代入
        percent = rate * 95;    //percentにrateに95かけた数を代入(0〜95)
        printf("%.0f\n",percent);
        if(percent != 0){   //バグ防止処理
        }else { 
            softPwmWrite(R,0);
            softPwmWrite(L,0);
        }
        if(flag == 1){      //flagが1(=負)のとき35番ピンを傾き分出力
            softPwmWrite(R,percent);
            delay(3);
        }else {             //flagが1以外すなわち0(=正)のとき37番ピンを傾き分出力
            softPwmWrite(L,percent);    
            delay(3);
        }
        usleep(10000);
    }
    return 0;
}

