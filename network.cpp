#include"network.hpp"

int Netcom::set(const char *IP, int PORT)
{
    server_ip=IP;
    server_port=PORT;
}

int Netcom::sock_connect(const char *IP, int PORT)
{
    struct sockaddr_in  servaddr;

    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
        return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, IP, &servaddr.sin_addr) <= 0)
    {
        printf("inet_pton error for %s\n", IP);
        return -1;
    }

    if (connect(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("connect error: %s(errno: %d)\n", strerror(errno), errno);
        return -1;
    }
    else
    {
        printf("connect successful!\n");
    }

}

int Netcom::sock_send()
{
    char buff[100]={0};
    if(send(socket_fd,buff,sizeof(buff)+sizeof(char),0)<0)
       std::cout<<"send error"<<std::endl;
}

int Netcom::sock_receive()
{
    ;
}

int Netcom::sock_close()
{
    close(socket_fd);
}

//mat transmission
int Matcom::mat_trans(cv::Mat image)
{
    if (image.empty())
    {
        printf("empty image\n\n");
        return -1;
    }

    if(image.cols != IMG_WIDTH || image.rows != IMG_HEIGHT || image.type() != CV_8UC3)
    {
        printf("the image must satisfy : cols == IMG_WIDTH（%d）  rows == IMG_HEIGHT（%d） type == CV_8UC3\n\n", IMG_WIDTH, IMG_HEIGHT);
        return -2;
    }

    for(int k = 0; k < PACKAGE_NUM; k++)
    {
        int num1 = IMG_HEIGHT / PACKAGE_NUM * k;
        for (int i = 0; i < IMG_HEIGHT / PACKAGE_NUM; i++)
        {
            int num2 = i * IMG_WIDTH * 3;
            uchar* ucdata = image.ptr<uchar>(i + num1);
            for (int j = 0; j < IMG_WIDTH * 3; j++)
            {
                data.buf[num2 + j] = ucdata[j];
            }
        }

        if(k == PACKAGE_NUM - 1)
            data.flag = 2;
        else
            data.flag = 1;

    int send_flag;
    if(send_flag=send(socket_fd, (char *)(&data), sizeof(data), 0)<0)
        throw(send_flag);
    }
    return 0;
}

