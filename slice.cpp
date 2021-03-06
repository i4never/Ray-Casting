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

void Slice::Z_Verse()
{
    Matrix result;
    result.Init(mat->r,mat->c);
    for (int i = 0 ; i < mat->r ; i++)
        for (int j = 0 ; j < mat->c ; j++)
            result.elmt[i][j] = mat->elmt[mat->r-1-i][mat->c-1-j];
    mat = &result;
}