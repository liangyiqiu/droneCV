#include "gpio.hpp"

int Gpio::init(int baud)
{
    wiringPiSetup();
    pinMode(0,OUTPUT);//RED LIGHT
    digitalWrite(0,HIGH);
    pinMode(2,OUTPUT);//GREEN LIGHT
    digitalWrite(2,HIGH);
    pinMode(3,OUTPUT);//BLUE LIGHT
    digitalWrite(3,HIGH);
    pinMode(7,OUTPUT);//BEEP
    digitalWrite(7,HIGH);
    pinMode(27,INPUT);//KEY 1
    pullUpDnControl(27,PUD_UP);
    pinMode(28,INPUT);//KEY 2
    pullUpDnControl(28,PUD_UP);
    pinMode(29,INPUT);//KEY 3
    pullUpDnControl(29,PUD_UP);
    fd=serialOpen("/dev/ttyAMA0",baud);
    baud_rate=baud;
    std::cout<<"gpio initialized!"<<std::endl;
    show_baud_rate();
    return 0;
}

int Gpio::setbit(int io,int action)
{
    if (action==1)
    {
        digitalWrite(io,HIGH);
    }
    else
    {
        digitalWrite(io,LOW);
    }
}

int Gpio::serial_receive()
{
    int height;
    char height_array[10]={0};
    if(serialDataAvail(fd))
    {
        for(int i=0;i<4;i++)
        {
            height_array[i]=serialGetchar(fd)-48;
        }
        if(height_array[3]=='H'-48)
            height=height_array[0]*100+height_array[1]*10+height_array[2];
    }
    serialFlush(fd);
    return height;
}
int Gpio::serial_transmit(int status,float x,float y,float z)
{
    status='M';//M/R/F not working
    char x_p[4]={0};
    char y_p[4]={0};
    char z_p[4]={0};

    x_p[0]= ((int)x)/100+48;
    x_p[1]= ((int)x)/10%10+48;
    x_p[2]= ((int)x)%10+48;

    y_p[0]= ((int)y)/100+48;
    y_p[1]= ((int)y)/10%10+48;
    y_p[2]= ((int)y)%10+48;

    z_p[0]= ((int)z)/100+48;
    z_p[1]= ((int)z)/10%10+48;
    z_p[2]= ((int)z)%10+48;

    serialPrintf(fd,"A%cx%sy%sz%s",status,x_p,y_p,z_p);
    return 0;
}

void Gpio::show_baud_rate()
{
    std::cout<<"baud_rate: "<<baud_rate<<std::endl;
}

int Gpio::scan()
{
    //std::cout<<"scanning"<<std::endl;
    if(digitalRead(27)==LOW)
    {
        usleep(20000);
        if(digitalRead(27)==LOW)
        {
            std::cout<<"task start"<<std::endl;
            task_flag=0;
            usleep(500000);
        }
    }

    if(digitalRead(28)==LOW)
    {
        usleep(20000);
        if(digitalRead(28)==LOW)
        {
            std::cout<<"28 pressed"<<std::endl;
            usleep(500000);
        }
    }

    if(digitalRead(29)==LOW)
    {
        usleep(20000);
        if(digitalRead(29)==LOW)
        {
            std::cout<<"29 pressed"<<std::endl;
            usleep(500000);
        }
    }
    usleep(10000);
}

void Light::blink(int color, int speed)
{
    int sleep_time;
    if(speed==SLOW)
    {
        sleep_time=800000;
    }
    else
    {
        sleep_time=400000;
    }

    digitalWrite(color,LOW);
    usleep(sleep_time);
    digitalWrite(color,HIGH);
    usleep(sleep_time);
}

void Light::turn_on(int color)
{
    switch (color)
    {
    case RED:
    {
        if(RED_status==OFF)
        {
            digitalWrite(RED,LOW);
            RED_status=ON;
        }
        break;
    }
    case GREEN:
    {
        if(GREEN_status==OFF)
        {
            digitalWrite(GREEN,LOW);
            GREEN_status=ON;
        }
        break;
    }
    case BLUE:
    {
        if(BLUE_status==OFF)
        {
            digitalWrite(BLUE,LOW);
            BLUE_status=ON;
        }
        break;
    }
    }
    usleep(10000);
}

void Light::turn_off(int color)
{
    switch (color)
    {
    case RED:
    {
        if(RED_status==ON)
        {
            digitalWrite(RED,HIGH);
            RED_status=OFF;
        }
        break;
    }
    case GREEN:
    {
        if(GREEN_status==ON)
        {
            digitalWrite(GREEN,HIGH);
            GREEN_status=OFF;
        }
        break;
    }
    case BLUE:
    {
        if(BLUE_status==ON)
        {
            digitalWrite(BLUE,HIGH);
            BLUE_status=OFF;
        }
        break;
    }
    }
    usleep(10000);
}
