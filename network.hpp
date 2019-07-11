#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <string>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "opencv2/opencv.hpp"

//normal net communication
class Netcom
{
public:
    const char* server_ip;
    int server_port;
    int socket_fd;
public:
    Netcom()
    {
        server_ip="192.168.43.113";
        server_port=6666;
        socket_fd=0;
    }
    int set(const char* IP,int PORT);
    int sock_connect(const char* IP,int PORT);
    int sock_send();
    int sock_receive();
    int sock_close();
};


//mat transmission
//待传输图像默认大小为 640*480，可修改
#define IMG_WIDTH 120	// 需传输图像的宽
#define IMG_HEIGHT 120	// 需传输图像的高
#define PACKAGE_NUM 10
//默认格式为CV_8UC3
#define BUFFER_SIZE IMG_WIDTH*IMG_HEIGHT*3/PACKAGE_NUM

struct sentbuf
{
    char buf[BUFFER_SIZE];
    int flag;
};

class Matcom: public Netcom
{
public:
    struct sentbuf data;
public:
    int mat_trans(cv::Mat image);
};

#endif // NETWORK_HPP
