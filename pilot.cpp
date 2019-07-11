#include"pilot.hpp"

int Pilot::init(int _fd)
{
    fd=_fd;
}

int Pilot::send()
{
    char status='M';//M/R/F not working
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

int Pilot::recv()
{
    char height_array[10]={0};
    if(serialDataAvail(fd))
    {
        for(int i=0;i<4;i++)
        {
            height_array[i]=serialGetchar(fd)-48;
        }
        if(height_array[3]=='H'-48)
        z_now=height_array[0]*100+height_array[1]*10+height_array[2];
    }
    serialFlush(fd);
    return 0;
}


