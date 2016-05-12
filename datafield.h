#include "slice.h"

class DataField
{
public:
    Slice ** slices;
    int num;
    double pixel_space;
    double limit[2][3];
public:
    DataField ();           //init
    ~DataField();           //destory
    bool Load(const char* path);    //load data
    bool Adjust();          //sort accroding t z-index move z-index to make the datafiled's center location is near (0,0,0)
    void Show();
};