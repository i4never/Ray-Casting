//#include "global.h"
//#include "slice.h"
#include "datafield.h"
#include <iostream>

using namespace std;

/******************************/
//class DataField
DataField::DataField()
{
    slices = NULL;
    num = 0;
    pixel_space = 0;
    z_step = 0;
}

DataField::~DataField()
{
    if (slices != NULL)
    {
        for (int i = 0 ; i < num ; i++)
            ;
        delete slices;
    }
}

bool DataField::Load(const char * path)
{
    FILE *fp = fopen(path,"r");
    
    //read file to get gray data
    fscanf(fp,"%d", &num);
    cout<<num<<endl;
    slices = new Slice*[num];
    cout<<"Start loading "<<num<<" pictures."<<endl;
    int r,c;
    for (int i = 0 ; i < num ; i++)
    {
        cout<<"Loading picture "<<i+1<<" ...";
        slices[i] = new Slice;
        slices[i]->mat = new Matrix;
        fscanf(fp,"%d   %d", &r, &c);
        fscanf(fp,"%lf", &(slices[i]->coordinate[0]));
        fscanf(fp,"%lf", &(slices[i]->coordinate[1]));
        fscanf(fp,"%lf", &(slices[i]->coordinate[2]));
        slices[i]->mat->Init(r,c);
        for (int j = 0 ; j < r ; j++)
            for (int k = 0 ; k < c ; k++)
                fscanf(fp,"%lf",&(slices[i]->mat->elmt[j][k]));
        cout<<"height:"<<r<<"   width:"<<c<<"   coordinate:"<<slices[i]->coordinate[0]<<"   "<<slices[i]->coordinate[1]<<"   "<<slices[i]->coordinate[2]<<"   "<<endl;
        slices[i]->Matrix2Image();
    }
    fscanf(fp,"%lf", &pixel_space);
    cout<<"Finish loading "<<num<<" pictures, pixel spacing is "<<pixel_space<<"."<<endl;
    Adjust();
    return true;
}

bool DataField::LoadTestData()
{
    double d = 100;
    z_step = 0.25;
    num = 200/z_step+1;
    pixel_space = 1;
    slices = new Slice*[num];
    int r,c;
    r = c = 200;
    for (int i = 0 ; i < num ; i++)
    {
        cout<<"slice "<<i<<endl;
        slices[i] = new Slice;
        slices[i]->mat = new Matrix;
        slices[i]->coordinate[0] = -d;
        slices[i]->coordinate[1] = -d;
        double z;
        z = slices[i]->coordinate[2] = -100+z_step*i;
        slices[i]->mat->Init(r,c);
        if  (slices[i]->coordinate[2] < 10 && slices[i]->coordinate[2] > -10)
            continue;
        for (int j = 0 ; j < r ; j++)
            for (int k = 0 ; k < c ; k++)
                if (((j-100)*(j-100)+(k-100)*(k-100)+z*z) > d*d)
                    slices[i]->mat->elmt[j][k] = 0;
                else
//                    if (z < 0)
//                        slices[i]->mat->elmt[j][k] = 100;
//                    else
//                        slices[i]->mat->elmt[j][k] = 200;
//                    slices[i]->mat->elmt[j][k] = 255 - 255*((j-100)*(j-100)+(k-100)*(k-100)+z*z)/(d*d)
                {
                    if (((j-100)*(j-100)+(k-100)*(k-100)+z*z) < d*d && ((j-100)*(j-100)+(k-100)*(k-100)+z*z) > 90*90)
                        slices[i]->mat->elmt[j][k] = 255;
                    if (((j-100)*(j-100)+(k-100)*(k-100)+z*z) < 40*40)
                        slices[i]->mat->elmt[j][k] = 100;
                }
    }
    limit[0][0] = -100;
    limit[0][1] = -100;
    limit[0][2] = -100;
    limit[1][0] = 99;
    limit[1][1] = 99;
    limit[1][2] = 100;
    cout<<"Test data has been loaded"<<endl;
    
    return true;
}

bool DataField::Adjust()
{
    //sort accroding to z-index
    for (int i = 0 ; i < num-1 ; i++)
        for (int j = i+1 ; j < num ; j++)
        {
            if (slices[i]->coordinate[2] > slices[j]->coordinate[2])
            {
                Slice * ptr = slices[i];
                slices[i] = slices[j];
                slices[j] = ptr;
            }
        }
    
    //move z-index
    int middle_index = num/2;
    double offset = slices[middle_index]->coordinate[2];
    for (int i = 0 ; i < num ; i ++)
        slices[i]->coordinate[2] -= offset;
    //x
    limit[0][0] = slices[0]->coordinate[0];
    limit[1][0] = slices[0]->coordinate[0]+pixel_space*(slices[0]->mat->r-1);
    //y
    limit[0][1] = slices[0]->coordinate[1];
    limit[1][1] = slices[0]->coordinate[1]+pixel_space*(slices[1]->mat->r-1);
    //z
    limit[0][2] = slices[0]->coordinate[2];
    limit[1][2] = slices[num-1]->coordinate[2];
    
    //z-step
    z_step = slices[1]->coordinate[2] - slices[0]->coordinate[2];
    return true;
}

void DataField::Show()
{
    cout<<"Total:"<<num<<endl;
    for (int i = 0 ; i < num ; i++)
    {
        cout<<"Pictrure "<<i<<"   height:"<<slices[i]->mat->r<<"   width:"<<slices[i]->mat->c<<"   coordinate:";
        cout<<slices[i]->coordinate[0]<<"   "<<slices[i]->coordinate[1]<<"   "<<slices[i]->coordinate[2]<<"   "<<endl;
    }
    cout<<"pixel spacing is: "<<pixel_space<<endl;
    cout<<"z step is: "<<z_step<<endl;
    cout<<"x limit is from "<<limit[0][0]<<" to "<<limit[1][0]<<endl;
    cout<<"y limit is from "<<limit[0][1]<<" to "<<limit[1][1]<<endl;
    cout<<"z limit is from "<<limit[0][2]<<" to "<<limit[1][2]<<endl;
}

bool DataField::Is_InField(Matrix coor)
{
    if (coor.elmt[0][0] <= limit[0][0] || coor.elmt[0][0] >= limit[1][0])
        return false;
    if (coor.elmt[0][1] <= limit[0][1] || coor.elmt[0][1] >= limit[1][2])
        return false;
    if (coor.elmt[0][2] <= limit[0][2] || coor.elmt[0][2] >= limit[1][2])
        return false;
    return true;
}

//double DataField::GetValue(Matrix coor)
//{
//    double result;
//    double value[8];
//    if (!Is_InField(coor))
//        return -1.0;
//    
//    // from up right font, opposite clock direction
//    //find first coordinate
//    int x,y,z;  //index, not coordinate
//    x = int((coor.elmt[0][0] - limit[0][0])/pixel_space) + 1;
//    y = int((coor.elmt[0][1] - limit[0][1])/pixel_space) + 1;
//    z = int((coor.elmt[0][2]-limit[0][2])/z_step) + 1;
//    
////    cout<<"linear inter "<<x<<"    "<<y<<" "<<z<<endl;
//    value[0] = slices[z]->mat->elmt[x][y];
//    value[1] = slices[z]->mat->elmt[x-1][y];
//    value[2] = slices[z]->mat->elmt[x-1][y-1];
//    value[3] = slices[z]->mat->elmt[x][y-1];
//    value[4] = slices[z-1]->mat->elmt[x][y];
//    value[5] = slices[z-1]->mat->elmt[x-1][y];
//    value[6] = slices[z-1]->mat->elmt[x-1][y-1];
//    value[7] = slices[z-1]->mat->elmt[x][y-1];
//    
////    for (int i = 0 ; i < 8 ; i++)
////        cout<<value[i]<<"   ";
////    cout<<endl;
//
//    double prop_x, prop_y, prop_z;
//    prop_x = (limit[0][0]+x*pixel_space-coor.elmt[0][0])/pixel_space;
//    prop_y = (limit[0][1]+y*pixel_space-coor.elmt[0][1])/pixel_space;
//    prop_z = (limit[0][2]+z*z_step-coor.elmt[0][2])/z_step;
////    cout<<"propotion: "<<prop_x<<"  "<<prop_y<<"    "<<prop_z<<endl;
//    
//    double v12,v03,v47,v56,vb,vf;
//    v12 = value[1]*(1-prop_y)+value[2]*prop_y;
//    v03 = value[0]*(1-prop_y)+value[3]*prop_y;
//    v47 = value[4]*(1-prop_y)+value[7]*prop_y;
//    v56 = value[5]*(1-prop_y)+value[6]*prop_y;
//    
//    vb = v12*(1-prop_z)+v56*prop_z;
//    vf = v03*(1-prop_z)+v47*prop_z;
//    
//    result = vf*(1-prop_x)+vb*prop_x;
//
////    cout<<"result:"<<result<<endl;
//    
//    
//    return result;
//}

double DataField::Linear_GetValue(Matrix coor)
{
    double result = 0;
    double value[8];
    if (!Is_InField(coor))
        return -1.0;
    
    // from up right font, opposite clock direction
    //find first coordinate
    int x,y,z;  //index, not coordinate
    x = int((coor.elmt[0][0] - limit[0][0])/pixel_space) + 1;
    y = int((coor.elmt[0][1] - limit[0][1])/pixel_space) + 1;
    z = int((coor.elmt[0][2] - limit[0][2])/z_step) + 1;
    
    //    cout<<"linear inter "<<x<<"    "<<y<<" "<<z<<endl;
    value[0] = slices[z]->mat->elmt[x][y];
    value[1] = slices[z]->mat->elmt[x-1][y];
    value[2] = slices[z]->mat->elmt[x-1][y-1];
    value[3] = slices[z]->mat->elmt[x][y-1];
    value[4] = slices[z-1]->mat->elmt[x][y];
    value[5] = slices[z-1]->mat->elmt[x-1][y];
    value[6] = slices[z-1]->mat->elmt[x-1][y-1];
    value[7] = slices[z-1]->mat->elmt[x][y-1];
    
    //    for (int i = 0 ; i < 8 ; i++)
    //        cout<<value[i]<<"   ";
    //    cout<<endl;
    
    double prop_x, prop_y, prop_z;
    prop_x = (limit[0][0]+x*pixel_space-coor.elmt[0][0])/pixel_space;
    prop_y = (limit[0][1]+y*pixel_space-coor.elmt[0][1])/pixel_space;
    prop_z = (limit[0][2]+z*z_step-coor.elmt[0][2])/z_step;
    //    cout<<"propotion: "<<prop_x<<"  "<<prop_y<<"    "<<prop_z<<endl;
    
    double v12,v03,v47,v56,vb,vf;
    v12 = value[1]*(1-prop_y)+value[2]*prop_y;
    v03 = value[0]*(1-prop_y)+value[3]*prop_y;
    v47 = value[4]*(1-prop_y)+value[7]*prop_y;
    v56 = value[5]*(1-prop_y)+value[6]*prop_y;
    
    vb = v12*(1-prop_z)+v56*prop_z;
    vf = v03*(1-prop_z)+v47*prop_z;
    
    result = vf*(1-prop_x)+vb*prop_x;
    
    //    cout<<"result:"<<result<<endl;
    
    
    return result;
}

double DataField::IDW_GetValue(Matrix coor)
{
    double result = 0;
    double value[8];
    if (!Is_InField(coor))
        return -1.0;
    
    // from up right font, opposite clock direction
    //find first coordinate
    int x,y,z;  //index, not coordinate
    x = int((coor.elmt[0][0] - limit[0][0])/pixel_space) + 1;
    y = int((coor.elmt[0][1] - limit[0][1])/pixel_space) + 1;
    z = int((coor.elmt[0][2]-limit[0][2])/z_step) + 1;
    
    //    cout<<"linear inter "<<x<<"    "<<y<<" "<<z<<endl;
    value[0] = slices[z]->mat->elmt[x][y];
    value[1] = slices[z]->mat->elmt[x-1][y];
    value[2] = slices[z]->mat->elmt[x-1][y-1];
    value[3] = slices[z]->mat->elmt[x][y-1];
    value[4] = slices[z-1]->mat->elmt[x][y];
    value[5] = slices[z-1]->mat->elmt[x-1][y];
    value[6] = slices[z-1]->mat->elmt[x-1][y-1];
    value[7] = slices[z-1]->mat->elmt[x][y-1];
    
    //    for (int i = 0 ; i < 8 ; i++)
    //        cout<<value[i]<<"   ";
    //    cout<<endl;
    
    double dis[8];
    dis[0] = (slices[z]->coordinate[0]+x*pixel_space-coor.elmt[0][0])*(slices[z]->coordinate[0]+x*pixel_space-coor.elmt[0][0]) +
             (slices[z]->coordinate[1]+y*pixel_space-coor.elmt[0][1])*(slices[z]->coordinate[1]+y*pixel_space-coor.elmt[0][1]) +
             (slices[z]->coordinate[2]-coor.elmt[0][2])*(slices[z]->coordinate[2]-coor.elmt[0][2]);
    
    dis[1] = (slices[z]->coordinate[0]+(x-1)*pixel_space-coor.elmt[0][0])*(slices[z]->coordinate[0]+(x-1)*pixel_space-coor.elmt[0][0]) +
             (slices[z]->coordinate[1]+y*pixel_space-coor.elmt[0][1])*(slices[z]->coordinate[1]+y*pixel_space-coor.elmt[0][1]) +
             (slices[z]->coordinate[2]-coor.elmt[0][2])*(slices[z]->coordinate[2]-coor.elmt[0][2]);

    dis[2] = (slices[z]->coordinate[0]+(x-1)*pixel_space-coor.elmt[0][0])*(slices[z]->coordinate[0]+(x-1)*pixel_space-coor.elmt[0][0]) +
             (slices[z]->coordinate[1]+(y-1)*pixel_space-coor.elmt[0][1])*(slices[z]->coordinate[1]+(y-1)*pixel_space-coor.elmt[0][1]) +
             (slices[z]->coordinate[2]-coor.elmt[0][2])*(slices[z]->coordinate[2]-coor.elmt[0][2]);
    
    dis[3] = (slices[z]->coordinate[0]+x*pixel_space-coor.elmt[0][0])*(slices[z]->coordinate[0]+x*pixel_space-coor.elmt[0][0]) +
             (slices[z]->coordinate[1]+(y-1)*pixel_space-coor.elmt[0][1])*(slices[z]->coordinate[1]+(y-1)*pixel_space-coor.elmt[0][1]) +
             (slices[z]->coordinate[2]-coor.elmt[0][2])*(slices[z]->coordinate[2]-coor.elmt[0][2]);
    
    dis[4] = (slices[z-1]->coordinate[0]+x*pixel_space-coor.elmt[0][0])*(slices[z-1]->coordinate[0]+x*pixel_space-coor.elmt[0][0]) +
             (slices[z-1]->coordinate[1]+y*pixel_space-coor.elmt[0][1])*(slices[z-1]->coordinate[1]+y*pixel_space-coor.elmt[0][1]) +
             (slices[z-1]->coordinate[2]-coor.elmt[0][2])*(slices[z-1]->coordinate[2]-coor.elmt[0][2]);
    
    dis[5] = (slices[z-1]->coordinate[0]+(x-1)*pixel_space-coor.elmt[0][0])*(slices[z-1]->coordinate[0]+(x-1)*pixel_space-coor.elmt[0][0]) +
             (slices[z-1]->coordinate[1]+y*pixel_space-coor.elmt[0][1])*(slices[z-1]->coordinate[1]+y*pixel_space-coor.elmt[0][1]) +
             (slices[z-1]->coordinate[2]-coor.elmt[0][2])*(slices[z-1]->coordinate[2]-coor.elmt[0][2]);
    
    dis[6] = (slices[z-1]->coordinate[0]+(x-1)*pixel_space-coor.elmt[0][0])*(slices[z-1]->coordinate[0]+(x-1)*pixel_space-coor.elmt[0][0]) +
             (slices[z-1]->coordinate[1]+(y-1)*pixel_space-coor.elmt[0][1])*(slices[z-1]->coordinate[1]+(y-1)*pixel_space-coor.elmt[0][1]) +
             (slices[z-1]->coordinate[2]-coor.elmt[0][2])*(slices[z-1]->coordinate[2]-coor.elmt[0][2]);
    
    dis[7] = (slices[z-1]->coordinate[0]+x*pixel_space-coor.elmt[0][0])*(slices[z-1]->coordinate[0]+x*pixel_space-coor.elmt[0][0]) +
             (slices[z-1]->coordinate[1]+(y-1)*pixel_space-coor.elmt[0][1])*(slices[z-1]->coordinate[1]+(y-1)*pixel_space-coor.elmt[0][1]) +
             (slices[z-1]->coordinate[2]-coor.elmt[0][2])*(slices[z-1]->coordinate[2]-coor.elmt[0][2]);

    double sum = 0;
    for (int i = 0 ; i < 8 ; i++)
    {
        dis[i] = 1/dis[i];
        sum += dis[i];
    }
    
    for (int i = 0 ; i < 8 ; i++)
        result += dis[i]*value[i]/sum;
    
    //    cout<<"result:"<<result<<endl;
    
    
    return result;
}


bool DataField::IDW(int per_layer)
{
//    Slice ** new_slices;
//    new_slices = new Slice*[num+per_layer*(num-1)];
//    double dz = z_step/(double)(per_layer+1);
//    num = num+per_layer*(num-1);
//    
//    Matrix coor;
//    coor.Init(1,3);
//    
//    int base = 0;
//    for (int i = 0 ; i < num ; i++)
//    {
//        base = i/(per_layer+1);
//        if (i%(per_layer+1) == 0)
//        {
//            new_slices[i] = slices[base];
//            continue;
//        }
//        new_slices[i] = new Slice;
//        new_slices[i]->mat = new Matrix;
//        new_slices[i]->mat->r = slices[base]->mat->r;
//        new_slices[i]->mat->c = slices[base]->mat->c;
//        new_slices[i]->coordinate[0] = slices[base]->coordinate[0];
//        new_slices[i]->coordinate[1] = slices[base]->coordinate[1];
//        new_slices[i]->coordinate[2] = slices[base]->coordinate[2]+dz*(i%per_layer+1);
//        new_slices[i]->mat->Init(r,c);
//        for (int j = 0 ; j < r ; j++)
//            for (int k = 0 ; k < c ; k++)
//            {
//                if (j*k == 0 || j == r-1 || k == c-1)
//                    new_slices[i]->mat->elmt[j][k] = slices[base]->elmt[j][k];
//                // IDW
//                else
//                {
//                    coor.elmt[0][0] = limit[0][0] + (j-1)*pixel_space;
//                    coor.elmt[0][1] = limit[0][1] + (k-1)*pixel_space;
//                    coor.elmt[0][2] = new_slices[i]->coordinate[2];
//                    double d[8];
//                    d[0] = (coor.elmt[0][2]-slices[base+1]->coordinate[2])*(coor.elmt[0][2]-slices[base+1]->coordinate[2]);
//                    d[1] = (coor.elmt[0][1])
//                }
//            }
////                fscanf(fp,"%lf",&(slices[i]->mat->elmt[j][k]));
//    }
//    
//    slices = new_slices;
    
    return true;
}