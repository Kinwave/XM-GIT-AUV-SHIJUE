#include "light_track.h"
#include "pi_usart.h"
#include "sort.h"

#define pi 3.1415926

using namespace cv;
using namespace std;


int g_nThresh_max = 255;
RNG g_rng(12345);
Mat g_cannyMat_output;
vector<vector<Point> > g_vContours;
vector<Vec4i> g_vHierarchy;


void on_ThreshChange(Mat& src)
{
	Canny(src, g_cannyMat_output, 80, 160, 3);
	// 寻找轮廓  
	findContours(g_cannyMat_output, g_vContours, g_vHierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));
	if (g_vContours.size() > 0)
	{
		vector <Point2f> center(g_vContours.size());
		vector <float> radius(g_vContours.size());
		vector <double> roundness(g_vContours.size());
		for (int i = 0; i < g_vContours.size(); i++)
		{
			//对给定的 2D 点集，寻找最小面积的包围圆		
			minEnclosingCircle(Mat(g_vContours[i]), center[i], radius[i]);
			double arclength = arcLength(g_vContours[i], true);
			roundness[i] = (4 * pi* contourArea(g_vContours[i])) / (arclength*arclength);//0.75-0.85多数表示等边多边形
		}

		Three_correlation_fQuickSort(roundness, center, radius, 0, g_vContours.size());


		Mat drawing = Mat::zeros(g_cannyMat_output.size(), CV_8UC3);
		Scalar color = Scalar(g_rng.uniform(0, 255), g_rng.uniform(0, 255), g_rng.uniform(0, 255));//任意值 
		drawContours(drawing, g_vContours, i, color, 1, 8, g_vHierarchy, 0, Point());

		//画出圆心  
		circle(drawing, center, 1, color, CV_FILLED, CV_AA);
		//绘制出最小面积的包围圆  
		circle(drawing, center, cvRound(radius), color, 1, CV_AA);

		// 显示效果图  
		namedWindow("result", 0);
		resizeWindow("result", 640, 480);
		imshow("result", drawing);
		cout << center;
	}
}

/*
void on_ThreshChange(Mat& src)
{
	// 用Canny算子检测边缘  
	Canny(src, g_cannyMat_output, 80, 160, 3);
	// 寻找轮廓  
	findContours(g_cannyMat_output, g_vContours, g_vHierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));
	//对给定的 2D 点集，寻找最小面积的包围圆  
    Point2f center[100];
    float radius[100] = {0};
	// 绘出轮廓  
	Mat drawing = Mat::zeros(g_cannyMat_output.size(), CV_8UC3);
	for (int i = 0; i < g_vContours.size(); i++)
	{


        minEnclosingCircle(Mat(g_vContours[i]), center[i], radius[i]);
	}

    if(g_vContours.size()>=1)
    {
    int max_radius =radius[0];
    int max_number=0;
    for(int i = 0; i < g_vContours.size(); i++)
    {
        if(radius[i]>max_radius)
        {
            max_radius=radius[i];
            max_number=i;
        }
    }
    Scalar color = Scalar(g_rng.uniform(0, 255), g_rng.uniform(0, 255), g_rng.uniform(0,255));//任意值
    //画出圆心
    drawContours(drawing, g_vContours, max_number, color, 1, 8, g_vHierarchy, 0, Point());
    //对给定的 2D 点集，寻找最小面积的包围圆
    circle(drawing, center[max_number], 1, color, CV_FILLED, CV_AA);
    //绘制出最小面积的包围圆
    circle(drawing, center[max_number], cvRound(radius[max_radius]), color, 1, CV_AA);
	// 显示效果图  
	namedWindow("result", 0);
	resizeWindow("result", 640, 480);
	imshow("result", drawing);
	cout << center;
    int xiangxian=0;
    int out_x;
    int out_y;
    if(center[max_number].x>=320)
    {
        if(center[max_number].y<=240)
        {
            xiangxian=1;
            out_x=int((center[max_number].x-320)/2);
            out_y=int((240-center[max_number].y)/2);
        }
        else
        {
            xiangxian=4;
            out_x=int((center[max_number].x-320)/2);
            out_y=int((center[max_number].y-240)/2);
        }
    }
    else
    {
        if(center[max_number].y<=240)
        {
            xiangxian=2;
            out_x=int((320-center[max_number].x)/2);
            out_y=int((240-center[max_number].y)/2);
        }
        else
        {
            xiangxian=3;
            out_x=int((320-center[max_number].x)/2);
            out_y=int((center[max_number].y-240)/2);
        }
    }
    usart_send(0x0B);
    usart_send(xiangxian);
    usart_send(out_x);
    usart_send(out_y);
    }
}
*/


