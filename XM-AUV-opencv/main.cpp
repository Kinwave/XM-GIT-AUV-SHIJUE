#include <opencv2/opencv.hpp> 
#include <iostream>  
#include <math.h>  
#include <time.h>  
#include <vector>

#include "main.h"

#include "pi_usart.h"
#include "network.h"

#include "image_processing.h"
#include "camera_basic.h"
#include "light_track.h"
#include "sort.h"


using namespace std;
using namespace cv;

int dock_flag=0;//1stop 2back

extern int xiangxian;
extern int out_x;
extern int out_y;

VideoWriter origin_writer("origin.avi", CV_FOURCC('D', 'I', 'V', 'X'), 25.0, Size(640, 480));
VideoWriter identify_writer("identify.avi", CV_FOURCC('D', 'I', 'V', 'X'), 25.0, Size(640, 480));
Mat read_film_result;

int main()
{
	#ifdef _if_run_by_pi
		usart_pthread_start(115200);
		network_pthread_start();
		ofstream opencvGet;
		opencvGet.open("opencv_get.txt");
	#endif

	Mat Origin_Image;
	Mat binnary_smooth_Image;
	
	//VideoCapture capture(1);//图像尺寸640,480
	VideoCapture capture("test.avi");

	while (1)
	{		
		Mat read_film;
		capture >> read_film;
		if (film_save_origin)
		{
			origin_writer << read_film;
		}
		read_film_result = read_film.clone();
		camera_read(read_film, Origin_Image, Size(640, 480), 20,read_image_show);
		image_processing(Origin_Image, binnary_smooth_Image, Size(640, 480), binary_image_show, smooth_image_show);
		on_ThreshChange(binnary_smooth_Image, result_image_show, origin_result_image_show);
		if (film_save_identify)
		{
			identify_writer << read_film_result;
		}

		#ifdef _if_run_by_pi
			opencvGet << xiangxian << " ";
			opencvGet << out_x << " ";
			opencvGet << out_y << endl;
		#endif

        waitKey(50);
	}
	return(0);
}



