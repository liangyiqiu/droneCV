#ifndef SYS_HPP
#define SYS_HPP

#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include "opencv2/opencv.hpp"
#include "gpio.hpp"
#include "network.hpp"
#include "timer.hpp"
#include "pilot.hpp"
#include "cv.hpp"

#define GOOD 1
#define BAD 0

#define DEBUG_MODE ('d')
#define RUN_MODE ('r')

class Sys
{
private:
    int network_status;
    int cam_status;
public:
    char boot_mode;

    Sys(int argc,char *argv[])
    {
        network_status=BAD;
        cam_status=BAD;
        if(argc==2&&*argv[1]==DEBUG_MODE)
            boot_mode=DEBUG_MODE;
        else
            boot_mode=RUN_MODE;
    }
    ~Sys()
    {
    }
    void Show_status()
    {
        if(boot_mode==DEBUG_MODE)
            std::cout<<"Debug mode"<<std::endl;
        else
            std::cout<<"Run mode"<<std::endl;
    }

    int init();
    int start();
    int wait();
};

extern Sys sys;
extern Netcom netcom;
extern Matcom matcom;
extern Pilot pilot;
extern Gpio gpio;

extern void *fun_light(void *argv);
extern void *fun_key(void *argb);
extern void *fun_pilot(void* argv);
extern void *fun_Mat_trans(void* argv);

#endif // SYS_HPP
