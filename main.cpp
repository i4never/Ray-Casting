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


//for test
bool DEBUG;


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

//GetGrayValue
//step: sample frequency
double GetGrayValue(Matrix coor_init, Matrix g, double step)
{
    double alpha_in = 0;
    double alpha = 0;
    double alpha_out = 0;
    double gray = 0;
    double gray_out = 0;
    Matrix coor;
    coor.Init(1,3);
    
    char main_direction;
    //judge the main dirction
    if (fabs(g.elmt[0][0]) > fabs(g.elmt[0][1]) && fabs(g.elmt[0][0]) > fabs(g.elmt[0][2]))
        main_direction = 'x';
    if (fabs(g.elmt[0][1]) > fabs(g.elmt[0][0]) && fabs(g.elmt[0][1]) > fabs(g.elmt[0][2]))
        main_direction = 'y';
    if (fabs(g.elmt[0][2]) > fabs(g.elmt[0][0]) && fabs(g.elmt[0][2]) > fabs(g.elmt[0][1]))
        main_direction = 'z';
    
//    cout<<"main_direction: "<<main_direction<<endl;
    
    int i = 0;
    while (true)
    {
        //calculate sample's coordinate
        coor.elmt[0][0] = coor_init.elmt[0][0]+i*step*g.elmt[0][0];
        coor.elmt[0][1] = coor_init.elmt[0][1]+i*step*g.elmt[0][1];
        coor.elmt[0][2] = coor_init.elmt[0][2]+i*step*g.elmt[0][2];
//        cout<<"sample "<<i<<": "<<coor.elmt[0][0]<<"    "<<coor.elmt[0][1]<<"   "<<coor.elmt[0][2]<<endl;
        i++;
        
        //GetSampleValue
        
        //loop control, if the coordiante is already outrange in the main direction, stop loop
        if (main_direction == 'x')
            if ((g.elmt[0][0] > 0 && coor.elmt[0][0] > data_field.limit[1][0]) || (g.elmt[0][0] < 0 && coor.elmt[0][0] < data_field.limit[0][0]))
                break;
        if (main_direction == 'y')
            if ((g.elmt[0][1] > 0 && coor.elmt[0][1] > data_field.limit[1][1]) || (g.elmt[0][1] < 0 && coor.elmt[0][1] < data_field.limit[0][1]))
                break;
        if (main_direction == 'z')
            if ((g.elmt[0][2] > 0 && coor.elmt[0][2] > data_field.limit[1][2]) || (g.elmt[0][2] < 0 && coor.elmt[0][2] < data_field.limit[0][2]))
                break;
        
        
        
        //judge while the pixel is in the datafield
        if (!data_field.Is_InField(coor))
            continue;
        
//        cout<<"sample "<<i<<": "<<coor.elmt[0][0]<<"    "<<coor.elmt[0][1]<<"   "<<coor.elmt[0][2]<<"   ";
        
        
        gray = data_field.GetValue(coor);
//        cout<<"in field,gray: "<<gray<<endl;
//        getchar();
//        //**************this is for test
//        if (gray)
//            return gray;
//        //**************this is for test

        
        //judge alpha according to the gray value
        //background
//        if (gray < 100)
//            continue;
        //else, linera relationship

//        if (gray < 107 || gray > 150)
//            continue;
        
//        alpha = (2*gray)/(double)215-(double)128/(double)215;
//        alpha = gray/(double)255;
        
//        if (gray != 255 && gray != 100)
//            continue;
//        if (gray == 255)
//            alpha = 0.1;
//        if (gray == 100)
//            alpha = 0.9;
//        if (gray == 50)
//            alpha = 0;
        
        
        //data lung bone
//        if (gray < 230)
//            continue;
//        alpha = (gray*9)/(double)1050-(double)83/(double)70;

        
        if (gray == 0)
            continue;
        alpha = -gray/(double)255+1;

        
        
//
//        
//        if (gray < 150)
//        {
//            continue;
//        }
        
        alpha_out = alpha_in + (1-alpha_in)*alpha;

        
        
        gray_out = (gray_out*alpha_in + gray*alpha*(1-alpha_in))/alpha_out;
//        cout<<"gray: "<<gray<<" alpha: "<<alpha<<" alpha out: "<<alpha_out<<" alpha in: "<<alpha_in<<" gray out: "<<gray_out<<endl;
        alpha_in =alpha_out;

    }

    return gray_out;
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
    u0.elmt[0][0] = 0;
    u0.elmt[0][1] = 1;
    u0.elmt[0][2] = 0;
    //init vertical direction
    v0.Init(1,3);
    v0.elmt[0][0] = 0;
    v0.elmt[0][1] = 0;
    v0.elmt[0][2] = 1;
    //init view direction
    g0.Init(1,3);
    g0.elmt[0][0] = -1;
    g0.elmt[0][1] = 0;
    g0.elmt[0][2] = 0;
    
    //after rotate
    Matrix u = u0*mat_rotate;
    Matrix v = v0*mat_rotate;
    Matrix g = g0*mat_rotate;
    cout<<"g:"<<endl;
    g.Show();
    
    //center location of the project image
    Matrix s;
    s = g*distance;
    ZERO(s.elmt[0][0]);
    ZERO(s.elmt[0][1]);
    ZERO(s.elmt[0][2]);
    cout<<"s:"<<endl;
    s.Show();
    
    //lower left location of the project image
    Matrix e;
    e = s-v*(res_height/2)-u*(res_width/2);
    cout<<"e:"<<endl;
    e.Show();
    
    
    Matrix coor;
    coor.Init(1,3);
    
    
    
    //*****test
//    while(true){
//        cin>>coor.elmt[0][0]>>coor.elmt[0][1]>>coor.elmt[0][2];
//        cout<<GetGrayValue(coor,g,data_field.pixel_space)<<endl;;
//    }
//
    
    //calculate each pixel
    int time = 0;
    for (int i = 0 ; i < res_height_pixel ; i++)
    {
        for (int j = 0 ; j < res_width_pixel ; j++)
        {
            coor = e+v*(i*data_field.pixel_space)+u*(j*data_field.pixel_space);
//            cout<<"******   "<<i<<" "<<j<<" "<<endl;
//            coor.Show();
//            result.elmt[i][j] = GetGrayValue();
            result.elmt[i][j] = GetGrayValue(coor,g,2);
//            cout<<"value: "<<result.elmt[i][j]<<endl;
//            getchar();
        }
//        getchar();
    }
    
    
    namedWindow( "Display window", WINDOW_NORMAL );
    
    Slice res;
    res.mat = &result;
//    res.Z_Verse();
    imshow( "Display window", res.Matrix2Image());
    
    waitKey(0);
    
    return result;
}

int main()
{
    double a = 0;
    double b = 0;
    cout<<a*b<<endl;
    Init();

    
    
    data_field.Load("./data");
    data_field.Adjust();
//    data_field.LoadTestData();
    data_field.Show();

    
    
//    
//    int current = 0;
//    namedWindow( "Display window", WINDOW_NORMAL );
//    imshow( "Display window", data_field.slices[current]->Matrix2Image());
//    
//    
//    
//
//    char c;
//    while(1)
//    {
//        c = waitKey(0);
//        cout<<c<<endl;
//        if (c == 'f')
//            current++;
//        if (c == 'b')
//            current--;
//        cout<<"current: "<<current<<endl;
//        imshow( "Display window", data_field.slices[current]->Matrix2Image());
//    }
    
   
//    while (true){
        int height,width,distance;
        cout<<"please input height , width , distance"<<endl;
        cin>>height>>width>>distance;
        cout<<"please input angel x, y, z"<<endl;
        double ang_x,ang_y,ang_z;
        cin>>ang_x>>ang_y>>ang_z;
    
        //弧度制，毫米
        RayCast(rate(ang_x),rate(ang_y),rate(ang_z),height,width,distance);
//    }
    
    return 0;
}
