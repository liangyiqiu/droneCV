#ifndef PILOT_HPP
#define PILOT_HPP

#include "gpio.hpp"

class Pilot
{
private:
    int fd;
public:
    //in centimetre
    int x;
    int y;
    int z;
    int z_now;
    //status
    int status;
    int status_now;
    int wait_time;
public:
    Pilot(int x_init,int y_init,int z_init,int wait_time_init)
    {
        wait_time=wait_time_init;
        x=x_init;
        y=y_init;
        z=z_init;
    }
    int init(int _fd);
    int send();
    int recv();
};

#endif // PILOT_HPP
