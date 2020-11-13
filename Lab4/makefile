cflags = -fopenmp
targets = omp_GEMM omp_schedule libpth4omp.so pth4omp_GEMM

all : $(targets)
.PHONY : clean all run

omp_GEMM : omp_GEMM.cpp matrix.hpp matrix.cpp
	g++ omp_GEMM.cpp matrix.cpp -o omp_GEMM $(cflags)
omp_schedule : omp_schedule.cpp matrix.hpp matrix.cpp
	g++ omp_schedule.cpp matrix.cpp -o omp_schedule $(cflags)
libpth4omp.so : pth4omp.hpp pth4omp.cpp
	g++ pth4omp.cpp -o libpth4omp.so -fPIC -lpthread -shared
pth4omp_GEMM : libpth4omp.so pth4omp_GEMM.cpp matrix.hpp matrix.cpp
	g++ pth4omp_GEMM.cpp matrix.cpp -o pth4omp_GEMM -L. -lpth4omp
run : pth4omp_GEMM
	LD_LIBRARY_PATH=. ./pth4omp_GEMM

clean : 
	-rm $(targets)
