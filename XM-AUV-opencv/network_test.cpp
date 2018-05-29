#include <sys/socket.h>  
#include <netinet/in.h>// sockaddr_in的头文件 
#include <arpa/inet.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h> //windows下要写windows.h 
#include <errno.h>  
#include <string.h>  
#include <sys/types.h>  
#include <time.h>  
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;
#define MAXLINE 2048

double x=3.5;
double y=6.2;
double z=0.5;
double head_angle=30.6;
double pitch_angle=-10.2;
double roll_angle=70.77;
  

int socket_AUV(int argc, char *argv[])  
{   
    time_t t = time(0);//初始化成当前时间！世界协调时（UTC:Universal Time Coordinated ）：年（y）、月（m）、日（d）、时（h）、分（min）、秒（s）
    ofstream location_AUV;//这时还没定义文件类型
    
    float Acc;
    float r_ENU;
    float GPS_x;
    float GPS_y;
    float GPS_Speed;
    float yaw_ENU;
    float dock_x;
    float dock_y;
    float time_hou;
    float time_min;
    float time_sec;

    char sendBuff[MAXLINE]; //发送的视觉求解出的姿态？？
    char rcvBuff[MAXLINE]; //STM32发来的传感器数据信息？
    int listenfd = 0, connfd = 0;//套接口描述符
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y%m%d%H%M%S.txt",localtime(&t) );//根据区域设置格式化本地时间/日期，函数的功能将时间格式化，或者说格式化一个时间字符串。

    char d[20]="./data/DOCK_";
    strcat(d,tmp);//将两个char类型连接。结果放在d中
    location_AUV.open(d, std::ios::out | std::ios::app); // ios::app：以追加的方式打开文件，即写进去的信息跟在原来后面，不覆盖;   ios::out：文件以输出方式打开（内存数据输出到文件）
  
    if (!location_AUV.is_open())
    {
      cout<<"Fail in open file!"<<endl;
      return 0;
    }
    
    //location_AUV<<setw(20)<<"Acceleration"<<setw(20)<<"Angular_velocity"<<setw(20)<<"GPS_x"<<setw(20)<<"GPS_y"<<setw(20)<<"GPS_Speed"<<setw(20)<<"yaw_ENU"<<setw(20)<<"dock_x"<<setw(20)<<"dock_y"<<setw(20)<<"time_hou"<<setw(20)<<"time_min"<<setw(20)<<"time_sec"<<endl;
    //最后要有endl，不然下次写会重在一起，数据没法和标题对齐
    location_AUV<<setw(20)<<"Acceleration"<<setw(20)<<"Angular_velocity"<<endl;
      
    struct sockaddr_in  serv_addr;//这句是声明，sockaddr_in 是系统自带的结构体。miniPC是当server的，STM32是client  
    
    
    listenfd = socket(AF_INET, SOCK_STREAM, 0); //第一个是地址描述，目前仅支持AF_INET格式；第二个是socket类型，如TCP（SOCK_STREAM）和UDP（SOCK_DGRAM）；第三个是套接口的协议，不指定的话可用0表示
    memset(&serv_addr, '0', sizeof(serv_addr));  //将serv_addr当前位置的后sizeof(serv_addr)个字节全部清0;     原型：void *memset(void *s, int ch, size_t n);
    memset(sendBuff, '0', sizeof(sendBuff));  //sizeof 返回一个对象或者类型所占的内存字节数 
  
    serv_addr.sin_family = AF_INET;  
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //INADDR_ANY指代任何地址 
    serv_addr.sin_port = htons(5000);   
  
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); //要将套接字和机器上的特定端口关联起来.函数原型：int bind(int sockfd, struct sockaddr *my_addr, int addrlen) ，
    printf("XXXXXXXXXXXXX");
  
    listen(listenfd, 10);   

    connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);//connf是新的套接字，这样就有两个套接字，原来的一个还在侦听端口，新的在准备发送send()和接收recv()数据..函数原型：int accept(int sockfd, void *addr, int *addrlen);

    while(1)  
    {  
        int len=0;

        snprintf(sendBuff, sizeof(sendBuff), "visual pose:%.5f %.5f %.5f %.5 %.5f %.5f\r\n",x,y,z,head_angle,pitch_angle,roll_angle); //函数原型为int snprintf(char *str, size_t size, const char *format, ...)。
        //将可变个参数(...)按照format格式化成字符串，然后将其复制到str中,%.5f：表示按浮点数输出，小数点后面取5位其余的舍去；例如：5/2 “%.5f”输出为：2.50000..如果：%9.5f 表示输出宽度为9的浮点数, 其中小数位为5, 整数位为3
        write(connfd, sendBuff, MAXLINE); //将sendbuff中的MAXLINE个字节内容写入文件描述符connfd.成功时返回写的字节数.失败时返回-1.	
        len=read(connfd,rcvBuff,MAXLINE);//返回值为实际读取到的字节数，如果返回0，表示已到达文件尾或是无可读取的数据.
	    cout<<"The length :"<<len<<endl;

        if(len>0)
        {
           rcvBuff[len]='\0';
           cout<<rcvBuff<<endl;
        }

        //sscanf(rcvBuff,"Start:%f %f %f %f %f %f %f %f %f %f %f End\r\n", &Acc,&r_ENU,&GPS_x,&GPS_y,&GPS_Speed,&yaw_ENU,&dock_x,&dock_y,&time_hou,&time_min,&time_sec);
        sscanf(rcvBuff,"Start:%f %f End\r\n", &Acc,&r_ENU);
        //分别是：前进线加速度，偏航角速度，GPS.X,  GPS.Y(东北天坐标系下），前进速度 ，航向角，dock的坐标x , y, 时，分，秒
        cout<<Acc<<endl;
        cout<<r_ENU<<endl;
       // cout<<GPS_x<<endl;
       // cout<<GPS_y<<endl;
	   // cout<<GPS_Speed<<endl;
	   // cout<<yaw_ENU<<endl;
	   // cout<<dock_x<<endl;
       // cout<<dock_y<<endl;	
       // cout<<time_hou<<endl;
	   // cout<<time_min<<endl;
        //cout<<time_sec<<endl;	

       // location_AUV<<setw(20)<<"Acc"<<setw(20)<<"r_ENU"<<setw(20)<<"GPS_x"<<setw(20)<<"GPS_y"<<setw(20)<<"GPS_Speed"<<setw(20)<<"yaw_ENU"<<setw(20)<<"dock_x"<<setw(20)<<"dock_y"<<setw(20)<<"time_hou"<<setw(20)<<"time_min"<<setw(20)<<"time_sec"<<endl;
       location_AUV<<setw(20)<<"Acc"<<setw(20)<<"r_ENU"<<endl;
     }  
     location_AUV.close();
}