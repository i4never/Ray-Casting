typedef int ELEMENT_TYPE;

class Matrix
{
public:
	ELEMENT_TYPE **elmt;
	int r;
	int c;
public:
	Matrix();
//    ~Matrix();
    void Init(int,int);                     //init
	void Reset();                           //clean the matrix
	void Show();                            //pirnt matrix
	Matrix operator*(Matrix multi);         //reload *
};