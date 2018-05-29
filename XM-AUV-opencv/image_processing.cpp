#include "image_processing.h"


void delete_jut(Mat& src, Mat& dst, int uthreshold, int vthreshold, int type);
void imageblur(Mat& src, Mat& dst, Size size, int threshold);

void image_processing(Mat& src, Mat& dst, Size size, int binary_image_show, int smooth_image_show)
{
	Mat gray_image;
	Mat g_gray_image;
	Mat erzhi_grayImage;
	Mat erzhi_tuchu_grayImage;
	Mat erzhi_pinghua_grayImage;
	// ת�ɻҶȲ�ģ��������
	cvtColor(src, gray_image, CV_BGR2GRAY);
	blur(gray_image, g_gray_image, Size(3, 3));
	//��ֵ������
	threshold(g_gray_image, erzhi_grayImage, 250, 255, THRESH_BINARY);
	if (binary_image_show==1)
	{
		namedWindow("binary", 0);
		resizeWindow("binary", size.width, size.height);
		imshow("binary", erzhi_grayImage);
	}
	delete_jut(erzhi_grayImage, erzhi_tuchu_grayImage, 20, 20, 1);
	imageblur(erzhi_tuchu_grayImage, dst, Size(3, 3), 150);
	if (smooth_image_show == 1)
	{
		namedWindow("smooth", 0);
		resizeWindow("smooth", size.width, size.height);
		imshow("smooth", dst);
	}
}

//ȥ����ֵͼ���Ե��ͻ����  
//uthreshold��vthreshold�ֱ��ʾͻ�����Ŀ����ֵ�͸߶���ֵ  
//type����ͻ��������ɫ��0��ʾ��ɫ��1�����ɫ
void delete_jut(Mat& src, Mat& dst, int uthreshold, int vthreshold, int type)
{
	src.copyTo(dst);
	int height = dst.rows;
	int width = dst.cols;
	int k;  //����ѭ���������ݵ��ⲿ  
	for (int i = 0; i < height - 1; i++)
	{
		uchar* p = dst.ptr<uchar>(i);
		for (int j = 0; j < width - 1; j++)
		{
			if (type == 0)
			{
				//������  
				if (p[j] == 255 && p[j + 1] == 0)
				{
					if (j + uthreshold >= width)
					{
						for (int k = j + 1; k < width; k++)
							p[k] = 255;
					}
					else
					{
						for (k = j + 2; k <= j + uthreshold; k++)
						{
							if (p[k] == 255) break;
						}
						if (p[k] == 255)
						{
							for (int h = j + 1; h < k; h++)
								p[h] = 255;
						}
					}
				}
				//������  
				if (p[j] == 255 && p[j + width] == 0)
				{
					if (i + vthreshold >= height)
					{
						for (k = j + width; k < j + (height - i)*width; k += width)
							p[k] = 255;
					}
					else
					{
						for (k = j + 2 * width; k <= j + vthreshold*width; k += width)
						{
							if (p[k] == 255) break;
						}
						if (p[k] == 255)
						{
							for (int h = j + width; h < k; h += width)
								p[h] = 255;
						}
					}
				}
			}
			else  //type = 1  
			{
				//������  
				if (p[j] == 0 && p[j + 1] == 255)
				{
					if (j + uthreshold >= width)
					{
						for (int k = j + 1; k < width; k++)
							p[k] = 0;
					}
					else
					{
						for (k = j + 2; k <= j + uthreshold; k++)
						{
							if (p[k] == 0) break;
						}
						if (p[k] == 0)
						{
							for (int h = j + 1; h < k; h++)
								p[h] = 0;
						}
					}
				}
				//������  
				if (p[j] == 0 && p[j + width] == 255)
				{
					if (i + vthreshold >= height)
					{
						for (k = j + width; k < j + (height - i)*width; k += width)
							p[k] = 0;
					}
					else
					{
						for (k = j + 2 * width; k <= j + vthreshold*width; k += width)
						{
							if (p[k] == 0) break;
						}
						if (p[k] == 0)
						{
							for (int h = j + width; h < k; h += width)
								p[h] = 0;
						}
					}
				}
			}
		}
	}
}

//ͼƬ��Ե�⻬����  
//size��ʾȡ��ֵ�Ĵ��ڴ�С��threshold��ʾ�Ծ�ֵͼ����ж�ֵ������ֵ  
void imageblur(Mat& src, Mat& dst, Size size, int threshold)
{
	int height = src.rows;
	int width = src.cols;
	blur(src, dst, size);
	for (int i = 0; i < height; i++)
	{
		uchar* p = dst.ptr<uchar>(i);
		for (int j = 0; j < width; j++)
		{
			if (p[j] < threshold)
				p[j] = 0;
			else p[j] = 255;
		}
	}
}
