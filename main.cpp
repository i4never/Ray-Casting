#include "datafield.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

const double EPS = 1e-10;
const double PI = 3.1415926535897932384626;
DataField data_field;

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

//Key function
Matrix RayCast(double ang_x, double ang_y, double ang_z, int res_height, int res_width, double distance)
{
    Matrix result;
    result.Init(res_height,res_width);

    Matrix u0,v0,g0;
    //rotate matrix
    Matrix mat_rotate = get_rotate(ang_x, ang_y, ang_z);
    //init horizon direction
    u0.Init(1,3);
    u0.elmt[0][0] = 1;
    u0.elmt[0][1] = 0;
    u0.elmt[0][2] = 0;
    //init vertical direction
    v0.Init(1,3);
    v0.elmt[0][0] = 0;
    v0.elmt[0][1] = 1;
    v0.elmt[0][2] = 0;
    //init view direction
    g0.Init(1,3);
    g0.elmt[0][0] = 0;
    g0.elmt[0][1] = 0;
    g0.elmt[0][2] = -1;
    
    //after rotate
    Matrix u = u0*mat_rotate;
    Matrix v = v0*mat_rotate;
    Matrix g = g0*mat_rotate;
    
    //middle coordinate of project image
    Matrix e;
    e.Init(1,3);
    
    
    
    
    return result;
}

int main()
{
   
	Init();

    
    
    data_field.Load("./data");
    
    
    namedWindow( "Display window", WINDOW_AUTOSIZE );
    
    int current = 0;
    imshow( "Display window", data_field.slices[current]->image);
    char in;
    while (1)
    {
        if (in == 'a')
            data_field.Adjust();
        in = waitKey(0);
        if (in != -1)
            cout<<in<<endl;
            if (in == 'q')
            break;
        if (in == 'f')
        {
            current = (current+1)%data_field.num;
            cout<<data_field.slices[current]->coordinate[0]<<"  "<<data_field.slices[current]->coordinate[1]<<"  "<<data_field.slices[current]->coordinate[2]<<"  "<<endl;
            namedWindow( "Display window", WINDOW_AUTOSIZE );
            imshow( "Display window", data_field.slices[current]->image);                   // Show our image inside it.
        }
            if (in == 'b')
        {
            current = (current-1)%data_field.num;
            cout<<"b    "<<current<<endl;
            namedWindow( "Display window", WINDOW_AUTOSIZE );
            imshow( "Display window", data_field.slices[current]->image);                   // Show our image inside it.
        }
            }
    
    
    
    //Init veison direction
    
//    RayCast(0,rate(180),0,1500,512,300);
    
    return 0;
}
