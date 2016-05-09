#include <stdlib.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

typedef int ELEMENT_TYPE;

class Matrix
{
public:
	ELEMENT_TYPE **elmt;
	int r;
	int c;
    Mat image;
public:
	Matrix();
//    ~Matrix();
    Mat Matrix2Image();                     //matrix 2 image
    void Init(int,int);                     //init
	void Reset();                           //clean the matrix
	void Show();                            //pirnt matrix
	Matrix operator*(Matrix multi);         //reload *
};