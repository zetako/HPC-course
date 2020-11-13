#!/bin/bash

make omp_schedule
echo '8 512 512 512' | ./omp_schedule
echo '8 1024 512 512' | ./omp_schedule
echo '8 1024 1024 512' | ./omp_schedule
echo '8 1024 1024 1024' | ./omp_schedule
echo '8 2048 1024 1024' | ./omp_schedule
echo '8 2048 2048 1024' | ./omp_schedule
echo '8 2048 2048 2048' | ./omp_schedule

echo '----------(@ ~ @)----------' >> omp_schedule.log

echo '1 1024 1024 1024' | ./omp_schedule
echo '2 1024 1024 1024' | ./omp_schedule
echo '4 1024 1024 1024' | ./omp_schedule
echo '6 1024 1024 1024' | ./omp_schedule
echo '8 1024 1024 1024' | ./omp_schedule