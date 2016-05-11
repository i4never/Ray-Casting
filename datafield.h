#include <stdlib.h>

class DataField
{
public:
    Matrix ** data;
    int num;
public:
    DataField ();           //init
    ~DataField();           //destory
    bool Load(const char* path);    //load data
};