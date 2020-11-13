#!/bin/bash

make pth4omp_GEMM
export LD_LIBRARY_PATH=.
echo '1 512 512 512' | ./pth4omp_GEMM
echo '1 1024 1024 1024' | ./pth4omp_GEMM
echo '1 2048 2048 2048' | ./pth4omp_GEMM
echo '2 512 512 512' | ./pth4omp_GEMM
echo '2 1024 1024 1024' | ./pth4omp_GEMM
echo '2 2048 2048 2048' | ./pth4omp_GEMM
echo '4 512 512 512' | ./pth4omp_GEMM
echo '4 1024 1024 1024' | ./pth4omp_GEMM
echo '4 2048 2048 2048' | ./pth4omp_GEMM
echo '8 512 512 512' | ./pth4omp_GEMM
echo '8 1024 1024 1024' | ./pth4omp_GEMM
echo '8 2048 2048 2048' | ./pth4omp_GEMM

