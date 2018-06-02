#include "image_processing.h"


void delete_jut(Mat& src, Mat& dst, int uthreshold, int vthreshold, int type);
void imageblur(Mat& src, Mat& dst, Size size, int threshold);
int otsu_threshold(Mat , double , int *);
Mat  ChangeImg(Mat &, int );


void image_processing(Mat& src, Mat& dst, Size size, int binary_image, int smooth_image)
{
	Mat gray_image;
	Mat g_gray_image;
	Mat erzhi_grayImage;
	Mat erzhi_tuchu_grayImage;
	Mat erzhi_pinghua_grayImage;
	// 转成灰度并模糊化降噪
	cvtColor(src, gray_image, CV_BGR2GRAY);
	blur(gray_image, g_gray_image, Size(3, 3));
	//二值化操作
	//Scalar average_light=mean(g_gray_image);
	//int light_digree = average_light[0]+80;
	threshold(g_gray_image, erzhi_grayImage, 240, 255, THRESH_BINARY);

	//二值化处理
	/*int Otsu_Offset = 20;
	int kmax_Offset = 5;
	int Thre[2];
	//int64 t2 = getTickCount();
	otsu_threshold(g_gray_image, 0.7, Thre);
	int Thre_Used = (Thre[0] + Otsu_Offset) < (Thre[1] - kmax_Offset) ? (Thre[0] + Otsu_Offset) : (Thre[1] - kmax_Offset);
	if (abs(Thre_Used) > 255)
		Thre_Used = 240;
	erzhi_grayImage = ChangeImg(g_gray_image, Thre_Used);   //二值化
	std::cout << Thre[0] <<"  "<< Thre[1] << "  "<<Thre_Used << std::endl;*/

	if (binary_image)
	{
		namedWindow("binary", 0);
		resizeWindow("binary", size.width, size.height);
		imshow("binary", erzhi_grayImage);
	}
	delete_jut(erzhi_grayImage, erzhi_tuchu_grayImage, 20, 20, 1);
	imageblur(erzhi_tuchu_grayImage, dst, Size(3, 3), 150);
	if (smooth_image)
	{
		namedWindow("smooth", 0);
		resizeWindow("smooth", size.width, size.height);
		imshow("smooth", dst);
	}
}

//去除二值图像边缘的突出部  
//uthreshold、vthreshold分别表示突出部的宽度阈值和高度阈值  
//type代表突出部的颜色，0表示黑色，1代表白色
void delete_jut(Mat& src, Mat& dst, int uthreshold, int vthreshold, int type)
{
	src.copyTo(dst);
	int height = dst.rows;
	int width = dst.cols;
	int k;  //用于循环计数传递到外部  
	for (int i = 0; i < height - 1; i++)
	{
		uchar* p = dst.ptr<uchar>(i);
		for (int j = 0; j < width - 1; j++)
		{
			if (type == 0)
			{
				//行消除  
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
				//列消除  
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
				//行消除  
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
				//列消除  
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

//图片边缘光滑处理  
//size表示取均值的窗口大小，threshold表示对均值图像进行二值化的阈值  
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


int otsu_threshold(Mat gray, double alpha, int *return_data)
{
	int img_width;
	int img_height;
	int pixel_size;
	float P1, P2;
	float m1 = 0, m2 = 0, mg = 0;
	int k_otsu;  //最终找到的阈值  
	int k_min = 10, k_max = 255;  // 限定k的取值范围，防止发生零除错误
	float deltaTmp = 0.0, deltaMax = 0.0;  //类间方差  
	int n0_k = 0;   //即(n0 +n1 + n2 +...nk)  
	int nk_255 = 0;

	int nk_X_k = 0;  //   表示(0*n0 + 1*n1 +2*n2 ...k*nk )  
	int nk_X_k2 = 0;  //  表示  

	int pixel_count[256];   //存储0-255每个像素的个数 即ni  
	memset(pixel_count, 0, sizeof(pixel_count));

	img_width = gray.cols;
	img_height = gray.rows;
	pixel_size = img_width * img_height;

	/*for (int i = 0; i<256; i++)
	{
	pixel_count[i] = 0;
	}*/

	//归一化直方图，统计每个像素点个数   即pi  
	for (int i = 0; i<img_height; i++)
	{
		for (int j = 0; j<img_width; j++)
		{
			pixel_count[gray.at<unsigned char>(i, j)]++;
		}
	}

	//计算mg  ， mg为总的像素平均灰度值  
	for (int i = 0; i<256; i++)
	{
		//cout<<i<<"="<<pixel_count[i]<<endl;
		mg += pixel_count[i] * i;
		if (i != 255)
		{
			if ((pixel_count[i] == 0) && (pixel_count[i + 1] != 0))
			{
				k_min = i + 1;
			}
			/*if ((pixel_count[i] != 0) && (pixel_count[i + 1] == 0))
			{
				k_max = i;
			}*/
		}
	}

	mg = mg / pixel_size;
	//k_min = k_min + 10;
	//k_max = k_max - 10;
	if (k_min > k_max)
	{
		return_data[0] = 10;
		return_data[1] = 10;
		return -1;
	}

	int n0_tmp = 0;
	for (int i = 230; i <256; i++)
	{
		n0_tmp += pixel_count[i];     //   P1 = n0_k  / pixel_size  
	}
	//cout<<"n0_tmp:  "<<n0_tmp<<"  pixel_size:  " <<pixel_size<<endl;
	//cout<<"High Ratio: "<<n0_tmp/float(pixel_size)<<endl;
	if (n0_tmp / float(pixel_size)  > 0.4)	//对于亮像素值较多的图像，采用较大的alpha值
		alpha += 0.15;

	for (int k = k_min; k<k_max; k++)    //类间方差统计  阈值k从0-255，筛选出最大方差的k  //根据实际情况，选择k的范围从50到250,因为较小的k值有可能会出现零除的错误 zt 2018.04.18
	{
		n0_k = 0;
		nk_X_k = 0;
		nk_X_k2 = 0;

		for (int i = 0; i <= k; i++)
		{
			n0_k += pixel_count[i];     //   P1 = n0_k  / pixel_size  
			nk_X_k += pixel_count[i] * i;  // m1 = nk_X_k / n0_k  
		}

		for (int j = k + 1; j<256; j++)   //计算m2  
		{
			nk_X_k2 += pixel_count[j] * j;

		}

		P1 = n0_k / (float)pixel_size;
		m1 = nk_X_k / n0_k;
		P2 = 1 - P1;
		m2 = nk_X_k2 / (float)(pixel_size - n0_k);

		deltaTmp = pow(P1, alpha)*(m1 - mg)*(m1 - mg) + pow(P2, 1 - alpha)*(m2 - mg)*(m2 - mg);
		//cout<<k<<"="<<deltaTmp<<endl;
		if (deltaTmp > deltaMax)
		{
			deltaMax = deltaTmp;
			k_otsu = k;
		}
	}
	return_data[0] = k_otsu;
	return_data[1] = k_max;
	return 0;
}

Mat  ChangeImg(Mat &img, int threshold)
{
	CV_Assert(img.depth() != sizeof(uchar));  //声明只对深度8bit的图像操作  
	int channels = img.channels();            //获取图像channel  
	int nrows = img.rows;                     //矩阵的行数  
	int ncols = img.cols*channels;             //矩阵的总列数=列数*channel分量数  
	if (img.isContinuous())               //判断矩阵是否连续，若连续，我们相当于只需要遍历一个一维数组  
	{
		ncols *= nrows;
		nrows = 1;                 //一维数组  
	}
	//遍历像素点灰度值  
	for (int i = 0; i<nrows; i++)
	{
		uchar *p = img.ptr<uchar>(i);    //获取行地址  
		for (int j = 0; j<ncols; j++)
		{
			if (p[j]<threshold)
				p[j] = 0;           //修改灰度值
			else
				p[j] = 255;

		}
	}
	return img;
}