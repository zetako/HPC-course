#!/bin/bash

nvcc CUBLAS_GEMM.cu check.cpp -o BLAS_GEMM -lcublas
echo '512 512 512' | ./BLAS_GEMM
echo '1024 1024 1024' | ./BLAS_GEMM
echo '2048 2048 2048' | ./BLAS_GEMM
echo '4096 4096 4096' | ./BLAS_GEMM
echo '8192 8192 8192' | ./BLAS_GEMM