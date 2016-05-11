//class RGB
//{
//public:
//	int r;
//	int g;
//	int b;
//};
//
//class Coordinate
//{
//public:
//	double x;
//	double y;
//	double z;
//public:
//	Coordinate();
//};
//
//class Linear_Inter
//{
//public:
//	double value1;
//	double value2;
//	double value;
//public:
//	Linear_Inter();				//init
//	int Calculate();			//calculate
//};


//
//int current = 0;
//imshow( "Display window", data_field.slices[current]->image);
//char in;
//while (1)
//{
//    in = waitKey(0);
//    if (in != -1)
//        cout<<in<<endl;
//        if (in == 'q')
//        break;
//        if (in == 'f')
//    {
//        current = (current+1)%data_field.num;
//        cout<<data_field.slices[current]->coordinate[0]<<"  "<<data_field.slices[current]->coordinate[1]<<"  "<<data_field.slices[current]->coordinate[2]<<"  "<<endl;
//        namedWindow( "Display window", WINDOW_AUTOSIZE );
//        imshow( "Display window", data_field.slices[current]->image);                   // Show our image inside it.
//    }
//        if (in == 'b')
//    {
//        current = (current-1)%data_field.num;
//        cout<<"b    "<<current<<endl;
//        namedWindow( "Display window", WINDOW_AUTOSIZE );
//        imshow( "Display window", data_field.slices[current]->image);                   // Show our image inside it.
//    }
//        }