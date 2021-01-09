#!/bin/bash

nvcc omp_CUDA_GEMM.cu check.cpp -Xcompiler -fopenmp -o omp_GEMM
echo '1 32 1024 1024 1024' | ./omp_GEMM
echo '1 32 2048 2048 2048' | ./omp_GEMM
echo '1 32 4096 4096 4096' | ./omp_GEMM
echo '1 512 1024 1024 1024' | ./omp_GEMM
echo '1 512 2048 2048 2048' | ./omp_GEMM
echo '1 512 4096 4096 4096' | ./omp_GEMM
echo '2 32 1024 1024 1024' | ./omp_GEMM
echo '2 32 2048 2048 2048' | ./omp_GEMM
echo '2 32 4096 4096 4096' | ./omp_GEMM
echo '2 512 1024 1024 1024' | ./omp_GEMM
echo '2 512 2048 2048 2048' | ./omp_GEMM
echo '2 512 4096 4096 4096' | ./omp_GEMM
echo '4 32 1024 1024 1024' | ./omp_GEMM
echo '4 32 2048 2048 2048' | ./omp_GEMM
echo '4 32 4096 4096 4096' | ./omp_GEMM
echo '4 512 1024 1024 1024' | ./omp_GEMM
echo '4 512 2048 2048 2048' | ./omp_GEMM
echo '4 512 4096 4096 4096' | ./omp_GEMM
echo '8 32 1024 1024 1024' | ./omp_GEMM
echo '8 32 2048 2048 2048' | ./omp_GEMM
echo '8 32 4096 4096 4096' | ./omp_GEMM
echo '8 512 1024 1024 1024' | ./omp_GEMM
echo '8 512 2048 2048 2048' | ./omp_GEMM
echo '8 512 4096 4096 4096' | ./omp_GEMM