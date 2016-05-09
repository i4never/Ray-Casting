#include "matrix.h"
#include <iostream>
#include <iomanip>

using namespace std;
using namespace cv;

/******************************/
//class Matrix
Matrix::Matrix()
{
	elmt = NULL;
	r = 0;
	c = 0;
}

//Matrix::~Matrix()
//{
////    if (elmt != NULL)
////    {
////        for (int i = 0 ; i < r ; i++)
////            delete elmt[i];
////        delete elmt;
////    }
//    ;
//}

void Matrix::Show()
{
	cout<<r<<"*"<<c<<" Matrix"<<endl;
	for (int i = 0 ; i < r ; i++)
	{
		for (int j = 0 ; j < c ; j++)
			cout<<setw(4)<<elmt[i][j];
		cout<<endl;
	}
}

Mat Matrix::Matrix2Image()
{
    image = Mat::Mat(r,c,CV_8UC4);
    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            Vec4b& rgba = image.at<Vec4b>(i, j);
            rgba[0] =(elmt[i][j]);
            rgba[1] = saturate_cast<uchar>(elmt[i][j]);
            rgba[2] = saturate_cast<uchar>(elmt[i][j]);
        }
    }
    return image;
}

void Matrix::Init(int r,int c)
{
	this->Reset();
	this->r = r;
	this->c = c;
	elmt = new ELEMENT_TYPE*[r];
	for (int i = 0 ; i < r ; i++)
		elmt[i] = new ELEMENT_TYPE[c];

	//this is for debug
	for (int i = 0 ; i < r ; i++)
		for (int j = 0 ; j < c ; j++)
			elmt[i][j] = 0;
}

Matrix Matrix::operator*(Matrix multi)
{
	if (this->c != multi.r)
	{
		cout<<"ERROR: The following matrixs can't be multipled."<<endl;
		this->Show();
		multi.Show();
		exit(-1);
	}

	Matrix result;
	result.Init(this->r,multi.c);
	for (int i = 0 ; i < this->r ; i++)
		for (int j = 0 ; j < multi.c ; j++)
		{
			result.elmt[i][j] = 0;
			for (int k = 0 ; k < multi.r ; k++)
				result.elmt[i][j] += this->elmt[i][k]*multi.elmt[k][j];
		}
	return result;
}

void Matrix::Reset()
{
	for (int i = 0 ; i < r ; i++)
		delete elmt[i];
	delete elmt;
	r = 0;
	c = 0;
}
