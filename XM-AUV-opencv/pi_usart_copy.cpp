#include "pi_usart.h"

void usart_pthread_start(int boud)
{
	if (-1 == wiringPiSetup())
	{
		cout << "setup error" << endl;
	}
	int fd;
	if ((fd = serialOpen("/dev/ttyAMA0", boud)) < 0)
	{
		printf("serial error\n");
	}
	serialPuts(fd, "ready");
	//serialPutchar(fd,serialGetchar(fd));
	//serialPutchar(fd, 0x12);
	
	pthread_t usart;
	int ser = pthread_create(&usart, NULL, usart_send, NULL);
	if (ser != 0)
	{
		std::cout << "pthread creat serial error" << std::endl;
	}
}


void usart_send(void)
{
	while (1)
	{
		int a = 2;
		a = 0;
	}
}