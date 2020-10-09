all:point2point collective

.PHONY:clean

point2point:mpi_GEMM.cpp matrix.hpp matrix.cpp
	mpic++ mpi_GEMM.cpp matrix.cpp -o point2point

collective:scatter_GEMM.cpp matrix.hpp matrix.cpp
	mpic++ scatter_GEMM.cpp matrix.cpp -o collective

clean:
	-rm point2point collective