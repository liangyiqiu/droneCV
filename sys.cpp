#include "sys.hpp"

pthread_t light_tid;
pthread_t key_tid;
pthread_t Mat_trans_tid;
pthread_t pilot_tid;
pthread_t task[5];
int argv;
int retval;

Sys sys(0,0);//arguments here have no use
Netcom netcom;
Matcom matcom;
Pilot pilot(59,59,0,100000);//init goal position is (59,59), 10 times a second
Gpio gpio;
Light light;

int Sys::init()
{
    gpio.init(115200);//baud rate is 115200
    pilot.init(gpio.fd);//use file pointer of object gpio
}

int Sys::start()
{
    pthread_create(&light_tid,NULL,fun_light,(void*)argv);
    pthread_create(&key_tid,NULL,fun_key,(void*)argv);
    pthread_create(&task[0],NULL,task0,(void*)argv);
    pthread_create(&pilot_tid,NULL,fun_pilot,(void*)argv);
    if(sys.boot_mode==DEBUG_MODE)
        pthread_create(&Mat_trans_tid,NULL,fun_Mat_trans,(void*)argv);
}

int Sys::wait()
{
    pthread_join(Mat_trans_tid,(void**)retval);
    pthread_join(task[0],(void**)retval);
    return 0;
}

void* fun_light(void *argv)
{
    while(1)
    {
        switch(task_flag)
        {
        case STANDBY:light.blink(GREEN,SLOW);break;
        case GROUND:light.blink(RED,FAST);break;
        default :light.turn_on(BLUE);light.turn_on(RED);
        }
    }
}


void *fun_key(void *argb)
{
    while(1)
    {
        gpio.scan();
    }
}

void *fun_pilot(void *argv)
{
    while(1)
    {
        usleep(pilot.wait_time);
        pilot.send();
        if(sys.boot_mode==DEBUG_MODE)
            std::cout<<"x:"<<pilot.x<<" y:"<<pilot.y<<" z:"<<pilot.z<<" z_now:"<<pilot.z_now<<std::endl;
        pilot.recv();
    }
}

void* fun_Mat_trans(void* argv)
{
    matcom.set("192.168.43.113",6666);
    if(matcom.sock_connect(matcom.server_ip,matcom.server_port)<0)
        return 0;
    sleep(5);
    std::cout<<"Mat trans begin"<<std::endl;
    while(1)
    {
        try
        {
            matcom.mat_trans(output_image);
        }
        catch(int)
        {
            std::cout<<"network broken"<<std::endl;
            matcom.sock_close();
            std::cout<<"trying to reconnect..."<<std::endl;
            matcom.sock_connect(matcom.server_ip,matcom.server_port);
        }
    }
}
