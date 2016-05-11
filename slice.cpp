#include "slice.h"

using namespace std;
using namespace cv;

/******************************/
//class Slice
Slice::Slice()
{
    mat = NULL;
    coordinate[0] = coordinate[1] = coordinate[2] = 0;
}

Mat Slice::Matrix2Image()
{
    image = Mat::Mat(mat->r,mat->c,CV_8UC4);
    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            Vec4b& rgba = image.at<Vec4b>(i, j);
            rgba[0] =(mat->elmt[i][j]);
            rgba[1] = saturate_cast<uchar>(mat->elmt[i][j]);
            rgba[2] = saturate_cast<uchar>(mat->elmt[i][j]);
        }
    }
    return image;
}
