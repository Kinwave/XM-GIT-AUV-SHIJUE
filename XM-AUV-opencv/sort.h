#ifndef sort_h

#include <opencv2/opencv.hpp> 
#include <vector>

using namespace std;
using namespace cv;

template<typename T>
void fQuickSort(vector<T> &A, int p, int r);

void Three_correlation_fQuickSort(vector<double> &sortA, vector<Point2f> &byA, vector<float> &byB, int p, int r);

#endif
