/*#include "pi_usart.h"
#include <iostream>
#include <math.h>
#include <time.h>
#include <vector>
#include "pthread.h"

using namespace std;

void *usart_read(void *arg);
extern int dock_flag;

int fd;

void usart_pthread_start(int boud)
{
	if (-1 == wiringPiSetup())
	{
		cout << "setup error" << endl;
	}

	if ((fd = serialOpen("/dev/ttyAMA0", boud)) < 0)
	{
        //printf("serial error\n");
	}
	serialPuts(fd, "ready");
	//serialPutchar(fd,serialGetchar(fd));
    serialPutchar(fd, 0x12);
	
    //usart_send(0x0B);

	pthread_t usart;
    int ser = pthread_create(&usart, NULL, usart_read, NULL);
	if (ser != 0)
	{
		std::cout << "pthread creat serial error" << std::endl;
	}
}

void *usart_read(void *arg)
{
    while(1)
    {
        int a=0;
        a=1;
        if(dock_flag==1)
        {
            usart_send(0x0B);
            usart_send(0X0C);
            usart_send(0X53);
            usart_send(0X4F);
            dock_flag=0;
        }
        else if(dock_flag==2)
        {
            usart_send(0x0B);
            usart_send(0X0C);
            usart_send(0X4F);
            usart_send(0X4F);
            dock_flag=0;
        }
    }
}



void usart_send(int send)
{
    serialPutchar(fd, send);
}

*/