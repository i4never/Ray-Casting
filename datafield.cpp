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
    FILE *fp = fopen("./data","r");
    
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
    return true;
}

bool DataField::Adjust()
{
    int middle_index = num/2;
    double offset = slices[middle_index]->coordinate[2];
    for (int i = 0 ; i < num ; i ++)
        slices[i]->coordinate[2] -= offset;
    return true;
}
