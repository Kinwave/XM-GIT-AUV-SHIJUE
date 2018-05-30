#include <opencv2/opencv.hpp> 
#include <iostream>  
#include <math.h>  
#include <time.h>  
#include <vector>

#include "pi_usart.h"
#include "network.h"

#include "image_processing.h"
#include "camera_basic.h"

#include "sort.h"


using namespace std;
using namespace cv;

int dock_flag=0;//1stop 2back

static void ShowHelpText();
void on_ThreshChange(Mat& src);

//XXXX_image_show 参数用于调试，1为显示，2为不显示
int main()
{

    //树莓派去掉该注释usart_pthread_start(115200);
	//树莓派去掉该注释network_pthread_start();

	ShowHelpText();

	int film_save_flag = 0;//1为开启录制视频模式
	VideoWriter writer("save.avi", CV_FOURCC('D', 'I', 'V', 'X'), 25.0, Size(640, 480));

	Mat Origin_Image;
	Mat binnary_smooth_Image;
	
	VideoCapture capture(1);//图像尺寸640,480

	//树莓派去掉该注释ofstream opencvGet;
	//树莓派去掉该注释opencvGet.open("opencv_get.txt");

	while (1)
	{		
		Mat read_film;
		capture >> read_film;
		if (film_save_flag==1)
		{
			writer << read_film;
			waitKey(20);
			continue;
		}
		camera_read(read_film, Origin_Image, Size(640, 480), 20, 1);
		waitKey(50);
		image_processing(Origin_Image, binnary_smooth_Image, Size(640, 480), 0, 1);
		//on_ThreshChange(binnary_smooth_Image);
		//树莓派去掉该注释opencvGet << xiangxian << " ";
		//树莓派去掉该注释opencvGet << out_x << " ";
		//树莓派去掉该注释opencvGet << out_y << endl;
        waitKey(50);
	}
	return(0);
}

//-----------------------------------【ShowHelpText( )函数】----------------------------------    
//      描述：输出一些帮助信息    
//----------------------------------------------------------------------------------------------    
static void ShowHelpText()
{
	//输出欢迎信息和OpenCV版本  
	printf("\n\n\t\t\t           AUV视觉识别\n");
	printf("\n\n\t\t\t           灯光中心跟踪\n");
	printf("\n\n\t\t\t   当前使用的OpenCV版本为：" CV_VERSION);
	printf("\n\n  ----------------------------------------------------------------------------\n");
}

