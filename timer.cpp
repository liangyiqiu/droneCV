#include "timer.hpp"
Timer::Timer(int arg_mode, double arg_time, int *arg_flag)
{
    mode=arg_mode;
    time=arg_time;
    flag=arg_flag;
    status=FREE;
}

Timer::~Timer()
{
    ;
}

int Timer::start()
{
    if(status==FREE)
    {
        pthread_create(&timer_tid,NULL,fun_timer,(void*)this);
        status=BUSY;
    }
}

void *Timer::fun_timer(void *__this)
{
    std::cout<<"timer start"<<std::endl;
    Timer *_this=(Timer*)__this;
    sleep(_this->time);
    *(_this->flag)=1;
}
