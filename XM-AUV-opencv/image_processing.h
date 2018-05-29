#ifndef image_processing_h

#include <opencv2/opencv.hpp> 

using namespace cv;

void image_processing(Mat& src, Mat& dst, Size size, int binary_image_show, int smooth_image_show);

#endif
