#include "slice.h"

class DataField
{
public:
    Slice ** slices;
    int num;
public:
    DataField ();           //init
    ~DataField();           //destory
    bool Load(const char* path);    //load data
};