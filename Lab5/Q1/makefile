cflags = -lm -lpthread -fopenmp
shared_obj = -L. -lpth4omp 
target = libpth4omp.so heated_plate
heated_plate_sources = heated_plate_my_omp.cpp heated_plate_work_func.hpp heated_plate_work_func.cpp

all : target
.PHONY : all clean run

libpth4omp.so : pth4omp.hpp pth4omp.cpp
	g++ pth4omp.cpp -o libpth4omp.so -fPIC -lpthread -shared

heated_plate : $(heated_plate_sources) libpth4omp.so
	g++ heated_plate_my_omp.cpp heated_plate_work_func.cpp -o heated_plate $(cflags) $(shared_obj)

run : heated_plate
	LD_LIBRARY_PATH=. ./heated_plate

clean : 
	-rm heated_plate libpth4omp.so