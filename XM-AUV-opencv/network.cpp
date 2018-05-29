/*#include <sys/socket.h>  
#include <netinet/in.h>
#include <arpa/inet.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h> 
#include <errno.h>  
#include <string.h>  
#include <sys/types.h>  
#include <time.h>  
#include <iostream>
#include <fstream>
#include <iomanip>

#include "network.h"
#include <pthread.h>
#include "pi_usart.h"

using namespace std;
#define MAXLINE 100
extern int dock_flag;

void *network_setup(void *arg);

void network_pthread_start(void)
{
	pthread_t network;
    int ser = pthread_create(&network, NULL, network_setup, NULL);
    //pthread_create(&network, NULL, network_setup, NULL);
    if (ser != 0)
	{
		std::cout << "pthread creat serial error" << std::endl;
    }

}
void *network_setup(void *arg)
{
    int send_back_stop[2]={0x53,0x4F};
    int send_back_out[2]={0x4F,0x4F};
	float a;
	float b;
	char sendBuff[MAXLINE]; //发送的数据
	char rcvBuff[MAXLINE]; //接收的数据
	int listenfd = 0, connfd = 0;//套接口描述符   
	struct sockaddr_in  serv_addr;//这句是声明，sockaddr_in 是系统自带的结构体。miniPC是当server的，STM32是client    
	listenfd = socket(AF_INET, SOCK_STREAM, 0); //第一个是地址描述，目前仅支持AF_INET格式；第二个是socket类型，如TCP（SOCK_STREAM）和UDP（SOCK_DGRAM）；第三个是套接口的协议，不指定的话可用0表示
	memset(&serv_addr, '0', sizeof(serv_addr));  //将serv_addr当前位置的后sizeof(serv_addr)个字节全部清0;     原型：void *memset(void *s, int ch, size_t n);
	memset(sendBuff, '0', sizeof(sendBuff));  //sizeof 返回一个对象或者类型所占的内存字节数 
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //INADDR_ANY指代任何地址 
	serv_addr.sin_port = htons(5000);//服务器监听端口号！ 
	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); //要将套接字和机器上的特定端口关联起来.函数原型：int bind(int sockfd, struct sockaddr *my_addr, int addrlen) ，
																	 //printf("XXXXXXXXXXXXX");

	listen(listenfd, 10);
	connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);//connf是新的套接字，这样就有两个套接字，原来的一个还在侦听端口，新的在准备发送send()和接收recv()数据..
	while (1)
	{
		int len = 0;

        //snprintf(sendBuff, sizeof(sendBuff), "visual pose:%.5f %.5f\r\n", x, y); //输入要发送的数据
        //write(connfd, sendBuff, MAXLINE); //将sendbuff中的MAXLINE个字节内容写入文件描述符connfd.成功时返回写的字节数.失败时返回-1.
		len = read(connfd, rcvBuff, MAXLINE);//返回值为实际读取到的字节数，如果返回0，表示已到达文件尾或是无可读取的数据.
		cout << "The length :" << len << endl;

		if (len > 0)
		{
			rcvBuff[len] = '\0';
			cout << rcvBuff << endl;
		}
        if(rcvBuff[0]==0x53)//s-stop
        {
            write(connfd, send_back_stop, MAXLINE);
            //usart_send(0x0B);
            //usart_send(0X0C);
            //usart_send(0X53);
            //usart_send(0X4F);
            dock_flag =1;
        }
        if(rcvBuff[0]==0x4F)//o-out
        {
            write(connfd, send_back_out, MAXLINE);
            //usart_send(0x0B);
            //usart_send(0X0C);
            //usart_send(0X4F);
            //usart_send(0X4F);
            dock_flag=2;
        }
	}

}*/
