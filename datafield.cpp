#include "global.h"
#include "datafield.h"
#include "matrix.h"
#include <iostream>

using namespace std;

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

bool DataField::Load(const char * path)
{
    FILE *fp = fopen("./data","r");
    
    //read file to get gray data
    fscanf(fp,"%d", &num);
    cout<<num<<endl;
    data = new Matrix*[num];
    cout<<"Start loading "<<num<<" pictures."<<endl;
    int r,c;
    for (int i = 0 ; i < num ; i++)
    {
        cout<<"Loading picture "<<i+1<<" ..."<<endl;
        data[i] = new Matrix;
        fscanf(fp,"%d   %d", &r, &c);
        data[i]->Init(r,c);
        for (int j = 0 ; j < r ; j++)
            for (int k = 0 ; k < c ; k++)
                fscanf(fp,"%d",&(data[i]->elmt[j][k]));
        data[i]->Matrix2Image();
    }
    cout<<"Finish loading "<<num<<" pictures."<<endl;
    return true;
}
