all:point2point

.PHONY:clean

point2point:mpi_GEMM.cpp matrix.hpp matrix.cpp
	mpic++ mpi_GEMM.cpp matrix.cpp -o point2point

clean:
	-rm point2point