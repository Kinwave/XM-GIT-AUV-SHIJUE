#ifdef _if_run_by_pi

	#include <iostream>
	#include <sstream>
	#include <fstream>
	#include <string.h>
	#include <math.h>
	#include <time.h>
	#include <vector>
	#include <string>
	#include <unistd.h>
	#include "pthread.h"
	#include "pi_usart.h"

	using namespace std;

	void *usart_read(void *);
	void stm32_data_writer(void);
	extern int dock_flag;

	string stm32_data_arrival;

	double stm32_depth;
	double stm32_angle;
	double stm32_angle_relative;
	double stm32_angle_error;
	double stm32_opencv_x;
	double stm32_opencv_y;
	int stm32_arrival_flag;

	int fd;
	static int length = 0;
	char test[100] = { 0 };

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

	//void *usart_read(void *arg)
	void *usart_read(void *)
	{
		//time_t t = time(0);
		//char tmp[64];
		//strftime(tmp, sizeof(tmp), "%Y%m%d%H%M%S.txt",localtime(&t) );//根据区域设置格式化本地时间/日期，函数的功能将时间格式化，或者说格式化一个时间字符串。
		//char d[20]="./";
		//strcat(d,tmp);//将两个char类型连接。结果放在d中
		ofstream outfile;
		outfile.open("stm32_get.txt");
		char stm32_data_head[] = "depth  angle  Rangle  Eangle  x  y";
		outfile << stm32_data_head << endl;
		while (1)
		{
			if (dock_flag == 1)
			{
				usart_send(0x0B);
				usart_send(0X0C);
				usart_send(0X53);
				usart_send(0X4F);
				dock_flag = 0;
			}
			else if (dock_flag == 2)
			{
				usart_send(0x0B);
				usart_send(0X0C);
				usart_send(0X4F);
				usart_send(0X4F);
				dock_flag = 0;
			}

			stm32_data_writer();
			if (stm32_arrival_flag == 1)
			{
				//outfile<<stm32_data_arrival<<endl;
				outfile << stm32_depth << ' ';
				outfile << stm32_angle << ' ';
				outfile << stm32_angle_relative << ' ';
				outfile << stm32_angle_error << ' ';
				outfile << stm32_opencv_x << ' ';
				outfile << stm32_opencv_y << ';' << endl;
				//outfile.close();
				stm32_data_arrival = "";
				//stm32_data_arrival_buf="";
				stm32_arrival_flag = 0;
				length = 0;
				memset(test, '\0', sizeof(test));
			}

		}
	}

	void stm32_data_writer(void)
	{
		int count_arrival = serialDataAvail(fd);
		//char test[100]="D3.212A53.312R231.312O231.234;";
		if (count_arrival > 0)
		{
			test[length] = serialGetchar(fd);
			length++;
			if (test[0] != 'D')
			{
				length = 0;
			}
			//test=serialGetchar(fd);
			//read(fd, test,count_arrival);
			//stm32_data_arrival = stm32_data_arrival + stm32_data_arrival_buf;
			//stm32_data_arrival="3.232T53.327T2873T231.378;";
			//stm32_arrival_flag=1;
		}
		if (test[length - 1] == ';')
		{
			stm32_data_arrival = test;
			char D, A, R, O, U, V, END;
			stringstream stm32_ansys(stm32_data_arrival);
			stm32_ansys >> D >> D >> stm32_depth >> A >> stm32_angle >> R >> stm32_angle_relative >> O >> stm32_angle_error >> U >> stm32_opencv_x >> V >> stm32_opencv_y >> END;
			stm32_arrival_flag = 1;
		}
		//outfile.put(linestring[i]);yizhong shuchu txt de xie fa
		//cout << linestring[i] << "";
	}

	void usart_send(int send)
	{
		serialPutchar(fd, send);
	}




#endif

