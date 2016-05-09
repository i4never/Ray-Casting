#include <stdlib.h>
#include <cmath>

class Matrix;
class DataField;

class RGB
{
public:
	int r;
	int g;
	int b;
};

class Coordinate
{
public:
	double x;
	double y;
	double z;
public:
	Coordinate();
};

class Linear_Inter
{
public:
	double value1;
	double value2;
	double value;
public:
	Linear_Inter();				//init
	int Calculate();			//calculate
};

class DataField
{
public:
    Matrix ** data;
    int num;
public:
    DataField ();           //init
    ~DataField();           //destory
};