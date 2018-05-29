#include "camera_basic.h"

void camera_read(Mat& src, Mat& dst, Size size, int camera_read_time, int read_image_show)
{
	// ����Դͼ��
	if (!src.data) 
	{
		printf("��ȡͼƬ������ȷ��Ŀ¼���Ƿ���imread����ָ����ͼƬ����~�� \n"); 
	}
	resize(src, dst, size);
	if (read_image_show==1)
	{
		namedWindow("��Ƶ", 0);
		resizeWindow("��Ƶ", size.width, size.height);
		imshow("��Ƶ", dst);
	}
	waitKey(camera_read_time);
}

