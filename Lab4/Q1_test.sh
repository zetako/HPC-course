#!/bin/bash

make omp_GEMM
echo '1 512 512 512' | ./omp_GEMM
echo '1 1024 1024 1024' | ./omp_GEMM
echo '1 2048 2048 2048' | ./omp_GEMM
echo '2 512 512 512' | ./omp_GEMM
echo '2 1024 1024 1024' | ./omp_GEMM
echo '2 2048 2048 2048' | ./omp_GEMM
echo '4 512 512 512' | ./omp_GEMM
echo '4 1024 1024 1024' | ./omp_GEMM
echo '4 2048 2048 2048' | ./omp_GEMM
echo '8 512 512 512' | ./omp_GEMM
echo '8 1024 1024 1024' | ./omp_GEMM
echo '8 2048 2048 2048' | ./omp_GEMM

