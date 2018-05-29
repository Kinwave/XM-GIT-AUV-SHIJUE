#include "camera_basic.h"

void camera_read(Mat& src, Mat& dst, Size size, int camera_read_time, int read_image_show)
{
	// 加载源图像
	if (!src.data) 
	{
		printf("读取图片错误，请确定目录下是否有imread函数指定的图片存在~！ \n"); 
	}
	resize(src, dst, size);
	if (read_image_show==1)
	{
		namedWindow("视频", 0);
		resizeWindow("视频", size.width, size.height);
		imshow("视频", dst);
	}
	waitKey(camera_read_time);
}

