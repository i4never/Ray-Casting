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
    return true;
}

void DataField::Show()
{
    cout<<"Total:"<<num<<endl;
    for (int i = 0 ; i < num ; i++)
    {
        cout<<"Pictrure "<<num<<"   height:"<<slices[i]->mat->r<<"   width:"<<slices[i]->mat->c<<"   coordinate:";
        cout<<slices[i]->coordinate[0]<<"   "<<slices[i]->coordinate[1]<<"   "<<slices[i]->coordinate[2]<<"   "<<endl;
    }
    cout<<"pixel spacing is:"<<pixel_space<<endl;
    cout<<"x limit is from "<<limit[0][0]<<" to "<<limit[1][0]<<endl;
    cout<<"y limit is from "<<limit[0][1]<<" to "<<limit[1][1]<<endl;
    cout<<"z limit is from "<<limit[0][2]<<" to "<<limit[1][2]<<endl;
}