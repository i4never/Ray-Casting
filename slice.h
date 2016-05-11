#include "matrix.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

class Slice
{
public:
    Matrix *mat;
    Mat image;
    double coordinate[3];
public:
	Slice();
//    ~Matrix();
    Mat Matrix2Image();                     //matrix 2 image
};