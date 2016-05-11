CC=g++

all:	touch matrix datafield interpolation coordinate main
	$(CC) matrix.o main.o datafield.o interpolation.o coordinate.o -L/usr/local/lib -lopencv_core.3.0.0 -lopencv_highgui.3.0.0 -lopencv_imgproc.3.0.0 -lopencv_imgcodecs.3.0.0 -o main.out
	rm -rf *.o
	./main.out

datafield:
	$(CC) -c datafield.cpp -I/usr/local/include

touch:
	touch *

matrix:	matrix.cpp matrix.h
	$(CC) -c matrix.cpp -I/usr/local/include

interpolation: interpolation.cpp global.h
	$(CC) -c interpolation.cpp

coordinate: coordinate.cpp global.h
	$(CC) -c coordinate.cpp

main:	main.cpp global.h
	$(CC) -c main.cpp -I/usr/local/include

clean:
	rm -rf *.out *.o
