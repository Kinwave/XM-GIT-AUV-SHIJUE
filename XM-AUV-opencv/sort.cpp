#include "sort.h"

template <typename T>
void fExchange(T &a, T &b)
{
	T temp = b;
	b = a;
	a = temp;
}

template<typename T>
void fQuickSort(vector<T> &A, int p, int r)
{
	if (p < r)
	{
		int q = fPartition(A, p, r);
		fQuickSort(A, p, q - 1);
		fQuickSort(A, q + 1, r);
	}
}


template<typename T>
int fPartition(T A[], int p, int r)
{
	srand(unsigned(time(0)));//选种子seed  

	int ran1 = (rand() % (r - p + 1)) + p;//产生[p,r]范围的随机数,取三个随机数的中间数  
	int ran2 = (rand() % (r - p + 1)) + p;
	int ran3 = (rand() % (r - p + 1)) + p;
	int ran;

	if (A[ran1] >= A[ran2] && A[ran1] <= A[ran3])
		ran = ran1;
	else if (A[ran2] >= A[ran1] && A[ran2] <= A[ran3])
		ran = ran2;
	else
		ran = ran3;


	T x = A[ran];
	fExchange(A[ran], A[r]);//将随机选出的数与A[r]交换  
	int i = p - 1;
	for (int j = p; j <= r - 1; j++)
	{
		if (A[j] <= x)
		{
			i++;
			fExchange(A[i], A[j]);
		}
	}
	fExchange(A[i + 1], A[r]);//将x放在数组中的顺序位置  
	for (int i = 0; i <= 11; i++)
		cout << A[i] << "  ";
	cout << endl;
	return i + 1;

}

//---------------------------------------------------------------------------------三联移动数组
void Three_correlation_fExchange(vector<double> &sortA, vector<Point2f> &byA, vector<float> &byB, int p, int r)
{
	double temp = sortA[r];
	sortA[r] = sortA[p];
	sortA[p] = temp;
	Point2f temp1 = byA[r];
	byA[r] = byA[p];
	byA[p] = temp1;
	float temp2 = byB[r];
	byB[r] = byB[p];
	byB[p] = temp2;
}


int Three_correlation_fPartition(vector<double> &sortA, vector<Point2f> &byA, vector<float> &byB, int p, int r)
{
	/*if (r==p)
	{
		return r;
	}
	else if ((r-p)==1)
	{
		if (sortA[p]<sortA[r])
		{
			Three_correlation_fExchange(sortA, byA, byB, r, p);
		}
		return r;
	}
	else {*/
		srand(unsigned(time(0)));

		int ran1 = (rand() % (r - p + 1)) + p;
		int ran2 = (rand() % (r - p + 1)) + p;
		int ran3 = (rand() % (r - p + 1)) + p;
		int ran;

		if ((sortA[ran1] >= sortA[ran2] && sortA[ran1] <= sortA[ran3]) || (sortA[ran1] <= sortA[ran2] && sortA[ran1] >= sortA[ran3]))
			ran = ran1;
		else if ((sortA[ran2] >= sortA[ran1] && sortA[ran2] <= sortA[ran3]) || (sortA[ran2] <= sortA[ran1] && sortA[ran2] >= sortA[ran3]))
			ran = ran2;
		else
			ran = ran3;


		double x = sortA[ran];
		Three_correlation_fExchange(sortA, byA, byB, ran, p);
		int right = r;
		int left = p;
		while (left < right)
		{
			while (sortA[right] <= x && right > left)
				right--;
			Three_correlation_fExchange(sortA, byA, byB, left, right);
			while (sortA[left] >= x && right > left)
				left++;
			Three_correlation_fExchange(sortA, byA, byB, left, right);
		}
		return left;
	//}
}

void Three_correlation_fQuickSort(vector<double> &sortA, vector<Point2f> &byA, vector<float> &byB, int p, int r)
{
	if (p < r)
	{
		int q = Three_correlation_fPartition(sortA, byA, byB, p, r);
		Three_correlation_fQuickSort(sortA, byA, byB, p, q - 1);
		Three_correlation_fQuickSort(sortA, byA, byB, q + 1, r);
	}
}

/*测试用
vector <Point2f> center(5);
vector <float> radius(5);
vector <double> roundness(5);

center[0].x = 323.23;
center[0].y = 3.13;
center[1].x = 3;
center[1].y = 3;
center[2].x = 5.2;
center[2].y = 2.32;
center[3].x = 23.21;
center[3].y = 312.2;
center[4].x = 3.123;
center[4].y = 4.31;
radius[0] = 12.23;
radius[1] = 23;
radius[2] = 21.123;
radius[3] = 3.24;
radius[4] = 5.23;
roundness[0] = 2432.21;
roundness[1] = 231.231;
roundness[2] = 12.23;
roundness[3] = 231.442;
roundness[4] = 423.2;



Three_correlation_fQuickSort(roundness, center, radius, 0, 4);

for (int i= 0;i<5;i++)
{
cout << "圆度" <<roundness[i]<<"\n" << endl;
cout << "中心" << center[i] << "\n" << endl;
cout << "半径" << radius[i] << "\n" << endl;
}

while(1)
{
waitKey(20);
}
*/