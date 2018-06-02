#include "light_track.h"
#include "pi_usart.h"
#include "sort.h"

#define pi 3.1415926

using namespace cv;
using namespace std;

int xiangxian;
int out_x;
int out_y;


int g_nThresh_max = 255;
RNG g_rng(12345);
Mat g_cannyMat_output;
vector<vector<Point> > g_vContours;
vector<Vec4i> g_vHierarchy;

extern Mat read_film_result;


void on_ThreshChange(Mat& src, int result_image,int orign_result_image)
{
	Canny(src, g_cannyMat_output, 80, 160, 3);
	// 寻找轮廓
	findContours(g_cannyMat_output, g_vContours, g_vHierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));
	if (g_vContours.size() > 0)
	{
		vector <Point2f> center(g_vContours.size());
		vector <float> radius(g_vContours.size());
		vector <double> roundness(g_vContours.size());
		int circle_select = 0;
		for (int i = 0; i < g_vContours.size(); i++)
		{
			//对给定的 2D 点集，寻找最小面积的包围圆
			minEnclosingCircle(Mat(g_vContours[i]), center[i], radius[i]);
			double arclength = arcLength(g_vContours[i], true);
			roundness[i] = (4 * pi* contourArea(g_vContours[i])) / (arclength*arclength);//0.75-0.85多数表示等边多边形
			if (_isnan(roundness[i]))
			{
				roundness[i]=0;
			}
		}		
		int number = g_vContours.size();
		Three_correlation_fQuickSort(roundness, center, radius, 0, number-1);
		while (center[circle_select].y < 160)
		{
			number--;
			if (number < 1)
			{
				break;
			}
			circle_select++;
		}
		if (number >= 1)
		{
			if (roundness[circle_select] > 0)
			{
				Mat drawing = Mat::zeros(g_cannyMat_output.size(), CV_8UC3);
				Scalar color = Scalar(g_rng.uniform(0, 255), g_rng.uniform(0, 255), g_rng.uniform(0, 255));//任意值
				drawContours(drawing, g_vContours, circle_select, color, 1, 8, g_vHierarchy, 0, Point());

				//画出圆心
				circle(drawing, center[circle_select], 1, color, CV_FILLED, CV_AA);
				//绘制出最小面积的包围圆
				circle(drawing, center[circle_select], cvRound(radius[circle_select]), color, 1, CV_AA);
				//画出圆心
				circle(read_film_result, center[circle_select], 5, color, CV_FILLED, CV_AA);
				//绘制出最小面积的包围圆
				circle(read_film_result, center[circle_select], cvRound(radius[circle_select]), color, 5, CV_AA);
				
				if (result_image)
				{
					// 显示效果图
					namedWindow("result", 0);
					resizeWindow("result", 640, 480);
					imshow("result", drawing);
					//cout << center;
				}

				if (orign_result_image)
				{
					// 显示效果图
					namedWindow("read_film_result", 0);
					resizeWindow("read_film_result", 640, 480);
					imshow("result", drawing);
					//cout << center;
				}

				//输出
				if (center[circle_select].x >= 320)
				{
					if (center[circle_select].y <= 240)
					{
						xiangxian = 1;
						out_x = int((center[circle_select].x - 320) / 2);
						out_y = int((240 - center[circle_select].y) / 2);
					}
					else
					{
						xiangxian = 4;
						out_x = int((center[circle_select].x - 320) / 2);
						out_y = int((center[circle_select].y - 240) / 2);
					}
				}
				else
				{
					if (center[circle_select].y <= 240)
					{
						xiangxian = 2;
						out_x = int((320 - center[circle_select].x) / 2);
						out_y = int((240 - center[circle_select].y) / 2);
					}
					else
					{
						xiangxian = 3;
						out_x = int((320 - center[circle_select].x) / 2);
						out_y = int((center[circle_select].y - 240) / 2);
					}
				}
				#ifdef _if_run_in_pi
					usart_send(0x0B);
					usart_send(xiangxian);
					usart_send(out_x);
					usart_send(out_y);
				#endif
			}
		}
	}
}










