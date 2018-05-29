#ifndef camera_basic_h

#include <opencv2/opencv.hpp> 

using namespace cv;

void camera_read(Mat& src, Mat& dst, Size size, int camera_read_time, int read_image_show);

#endif
