#ifndef GPIO_HPP
#define GPIO_HPP

#include "wiringPi.h"
#include "wiringSerial.h"
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>

extern int task_flag;

class Gpio
{
public:
    int fd;
    int baud_rate;
public:
    Gpio()
    {
    }
    ~Gpio()
    {
        serialClose(fd);
    }
    int init(int baud);
    int setbit(int io,int action);
    int serial_receive();
    int serial_transmit(int status,float x,float y,float z);
    void show_baud_rate();
    int scan();
};

#define RED 0
#define GREEN 2
#define BLUE 3

#define SLOW 0
#define FAST 1

#define ON 1
#define OFF 0

class Light
{
private:
    int RED_status;
    int GREEN_status;
    int BLUE_status;
public:
    Light()
    {
        RED_status=OFF;
        GREEN_status=OFF;
        BLUE_status=OFF;
    }
    void blink(int color,int speed);
    void turn_on(int color);
    void turn_off(int color);
};

#endif // GPIO_HPP
