cflags = -lpthread
builds = pthread_GEMM array array_mul equation monte_carlo

targets = pthread_GEMM array array_mul equation monte_carlo

all : $(targets)
.PHONY : clean all

pthread_GEMM : pthread_GEMM.cpp matrix.hpp matrix.cpp
	CXX pthread_GEMM.cpp matrix.cpp -o pthread_GEMM $(cflags)
array : array.cpp
	CXX array.cpp -o array $(cflags)
array_mul : array_mul.cpp
	CXX array_mul.cpp -o array_mul $(cflags)
equation : equation.cpp
	CXX equation.cpp -o equation -lm $(cflags)
monte_carlo : monteCarlo.cpp
	CXX monteCarlo.cpp -o monteCarlo $(cflags)

clean : 
	-rm $(targets)
