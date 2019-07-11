#ifndef TIMER_HPP
#define TIMER_HPP

#include <unistd.h>
#include <pthread.h>
#include <iostream>

#define FREE 0
#define BUSY 1

class Timer
{
private:
    int mode;
    int status;
    double time;
    int *flag;
    pthread_t timer_tid;
    int argv;
public:
    Timer(int arg_mode,double arg_time,int *arg_flag);
    ~Timer();
    int start();
    static void* fun_timer(void* __this);
    int show_status();
};

#endif // TIMER_HPP
