#include "slice.h"

class DataField
{
public:
    Slice ** slices;
    int num;
    double pixel_space;
    double z_step;
    double limit[2][3];
public:
    DataField ();                   //init
    ~DataField();                   //destory
    bool Load(const char* path);    //load data
    bool Adjust();                  //sort accroding t z-index move z-index to make the datafiled's center location is near (0,0,0)
    bool Is_InField(Matrix coor);   //judge while a coordinate is in the datafield
    void Show();
    double Linear_GetValue(Matrix coor);   //linear interplotion
    double IDW_GetValue(Matrix coor);   //idw interplotion
    bool LoadTestData();            //load a ball
    bool IDW(int per_layer);        //Inverse Distance Weighted
};