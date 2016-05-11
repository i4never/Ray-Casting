#include "datafield.h"
#include <cmath>
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

#define ZERO(a) if (abs(a)<EPS) a = 0;

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

//Key function (the angle are using 0~2pi, all the length is mm)
Matrix RayCast(double ang_x, double ang_y, double ang_z, double res_height, double res_width, double distance)
{
    //result image data
    int res_height_pixel = int(res_height/data_field.pixel_space+1);
    int res_width_pixel = int(res_width/data_field.pixel_space+1);
    cout<<res_height_pixel<<" "<<res_width_pixel<<endl;
    Matrix result;
    result.Init(res_height_pixel,res_width_pixel);

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
    
    //center location of the project image
    Matrix s;
    s = g*distance;
    ZERO(s.elmt[0][0]);
    ZERO(s.elmt[0][1]);
    ZERO(s.elmt[0][2]);
    s.Show();
    
    //lower left location of the project image
    Matrix e;
    e = s-v*(res_height/2)-u*(res_width/2);
    e.Show();
    
    Matrix coor;
    coor.Init(1,3);
    //calculate each pixel
    for (int i = 0 ; i < res_height_pixel ; i++)
    {
        for (int j = 0 ; j < res_width_pixel ; j++)
        {
            //calculate the coordinate of this pixel
//            coor.elmt[0] = e.elmt[0]+i*pixel_space*v[0]+j*pixel_space*u[0];
            coor = e+v*(i*data_field.pixel_space)+u*(j*data_field.pixel_space);
            if (coor.elmt[0][0]*coor.elmt[0][1]<0)
                result.elmt[i][j] = 255;
            else
                result.elmt[i][j] =0;
        }
    }
    coor.Show();
    
    namedWindow( "Display window", WINDOW_AUTOSIZE );
    
    Slice res;
    res.mat = &result;
    imshow( "Display window", res.Matrix2Image());
    
    waitKey(0);
    
    return result;
}

int main()
{
   
	Init();

    
    
    data_field.Load("./data");

    RayCast(0,0,0,1400,400,-300);
    
    return 0;
}
