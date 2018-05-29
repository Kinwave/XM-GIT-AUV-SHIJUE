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
	char sendBuff[MAXLINE]; //���͵�����
	char rcvBuff[MAXLINE]; //���յ�����
	int listenfd = 0, connfd = 0;//�׽ӿ�������   
	struct sockaddr_in  serv_addr;//�����������sockaddr_in ��ϵͳ�Դ��Ľṹ�塣miniPC�ǵ�server�ģ�STM32��client    
	listenfd = socket(AF_INET, SOCK_STREAM, 0); //��һ���ǵ�ַ������Ŀǰ��֧��AF_INET��ʽ���ڶ�����socket���ͣ���TCP��SOCK_STREAM����UDP��SOCK_DGRAM�������������׽ӿڵ�Э�飬��ָ���Ļ�����0��ʾ
	memset(&serv_addr, '0', sizeof(serv_addr));  //��serv_addr��ǰλ�õĺ�sizeof(serv_addr)���ֽ�ȫ����0;     ԭ�ͣ�void *memset(void *s, int ch, size_t n);
	memset(sendBuff, '0', sizeof(sendBuff));  //sizeof ����һ���������������ռ���ڴ��ֽ��� 
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //INADDR_ANYָ���κε�ַ 
	serv_addr.sin_port = htons(5000);//�����������˿ںţ� 
	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); //Ҫ���׽��ֺͻ����ϵ��ض��˿ڹ�������.����ԭ�ͣ�int bind(int sockfd, struct sockaddr *my_addr, int addrlen) ��
																	 //printf("XXXXXXXXXXXXX");

	listen(listenfd, 10);
	connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);//connf���µ��׽��֣��������������׽��֣�ԭ����һ�����������˿ڣ��µ���׼������send()�ͽ���recv()����..
	while (1)
	{
		int len = 0;

        //snprintf(sendBuff, sizeof(sendBuff), "visual pose:%.5f %.5f\r\n", x, y); //����Ҫ���͵�����
        //write(connfd, sendBuff, MAXLINE); //��sendbuff�е�MAXLINE���ֽ�����д���ļ�������connfd.�ɹ�ʱ����д���ֽ���.ʧ��ʱ����-1.
		len = read(connfd, rcvBuff, MAXLINE);//����ֵΪʵ�ʶ�ȡ�����ֽ������������0����ʾ�ѵ����ļ�β�����޿ɶ�ȡ������.
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
