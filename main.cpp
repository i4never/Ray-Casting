#include "global.h"
#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

const double EPS = 1e-10;
const double PI = 3.1415926535897932384626;
DataField data_field;


/******************************/
//class DataField
DataField::DataField()
{
    data = NULL;
    num = 0;
}

DataField::~DataField()
{
    if (data != NULL)
    {
        for (int i = 0 ; i < num ; i++)
            ;
        delete data;
    }
}

//change int to string
char * itoa(int num)
{
	char* result = new char[4];
	int pt = 0;
	while(num != 0)
	{
		result[pt++] = num%10+'0';
		num /= 10;
	}
	for (int i = pt-1 ; i>pt/2-1 ; i--)
	{
		char c = result[i];
		result[i] = result[pt-i-1];
		result[pt-i-1] =  c;
	}
	return result;
}

//change 
double rate(double rate)
{
	while (rate > 360)
		rate -= 360;
	while (rate < 0)
		rate += 360;	
	return rate*2*PI/360;
}

//init value
void Init()
{
	return;	
}

//get rotate matrix
Matrix get_rotate(double ang_x, double ang_y, double ang_z)
{
	Matrix R;
	R.Init(3,3);
	R.elmt[0][0] = cos(ang_y)*cos(ang_z);
	R.elmt[0][1] = cos(ang_y)*sin(ang_z);
	R.elmt[0][2] = -sin(ang_y);
	R.elmt[1][0] = sin(ang_x)*sin(ang_y)*cos(ang_z)-cos(ang_x)*sin(ang_z);
	R.elmt[1][1] = sin(ang_x)*sin(ang_y)*sin(ang_z)+cos(ang_x)*cos(ang_z);
	R.elmt[1][2] = sin(ang_x)*cos(ang_y);
	R.elmt[2][0] = cos(ang_x)*sin(ang_y)*cos(ang_z)+sin(ang_x)*sin(ang_z);
	R.elmt[2][1] = cos(ang_x)*sin(ang_y)*sin(ang_z)-sin(ang_x)*cos(ang_z);
	R.elmt[2][2] = cos(ang_x)*cos(ang_y);
	return R;
}

int main()
{
   
	Init();
	Matrix a = get_rotate(3.1415926535897932384626,0,0);
//	a.Show();

/*	Matrix b;
	b.Init(1,3);
	b.pt[0][0] = 0;
	b.pt[0][1] = 0;
	b.pt[0][2] = -1;
	double x,y,z;
	while (1)
	{
		cin>>x>>y>>z;
		(b*get_rotate(rate(x),rate(y),rate(z))).Show();
	}
*/
    
    FILE *fp = fopen("./data","r");
    
    //read file to get gray data
    fscanf(fp,"%d", &data_field.num);
    cout<<data_field.num<<endl;
    data_field.data = new Matrix*[data_field.num];
    
    cout<<"Start loading "<<data_field.num<<" pictures."<<endl;
    int r,c;
    for (int i = 0 ; i < data_field.num ; i++)
//        for (int i = 0 ; i < 1 ; i++)
    {
        cout<<"Loading picture "<<i+1<<" ..."<<endl;
        data_field.data[i] = new Matrix;
        fscanf(fp,"%d   %d", &r, &c);
        data_field.data[i]->Init(r,c);
        for (int j = 0 ; j < r ; j++)
            for (int k = 0 ; k < c ; k++)
                fscanf(fp,"%d",&(data_field.data[i]->elmt[j][k]));
        imshow( "Display window",data_field.data[i]->Matrix2Image());
//        data_field.data[i]->Show();
    }
    cout<<"Finish loading "<<data_field.num<<" pictures."<<endl;

//    int r,c;
//    Matrix temp;
//    fscanf(fp,"%d   %d", &r, &c);
//    temp.Init(r,c);
//    for (int i  = 0 ; i < r ; i++)
//        for (int j = 0 ; j < c ; j++)
//            fscanf(fp,"%d",&temp.elmt[i][j]);
//    temp.Show();
    
    namedWindow( "Display window", WINDOW_AUTOSIZE );
    imshow( "Display window", data_field.data[0]->image);                   // Show our image inside it.
    
    int current = 0;
    while (1)
    {
        char c = waitKey(0);
        if (c != -1)
            cout<<c<<endl;
        if (c == 'q')
            return 0;
        if (c == 'f')
        {
            cout<<"F"<<endl;
            current = (current+1)/data_field.num;
            namedWindow( "Display window", WINDOW_AUTOSIZE );
            imshow( "Display window", data_field.data[current]->image);                   // Show our image inside it.
        }
        if (c == 'b')
        {
            cout<<"b"<<endl;
            current = (current-1)/data_field.num;
            namedWindow( "Display window", WINDOW_AUTOSIZE );
            imshow( "Display window", data_field.data[current]->image);                   // Show our image inside it.
        }
    }
    
    return 0;
}
