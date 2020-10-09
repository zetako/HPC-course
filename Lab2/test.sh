#!/bin/bash

make point2point
echo '512 512 512' > arg.txt
mpirun -n 1 ./point2point < arg.txt
mpirun -n 2 ./point2point < arg.txt
mpirun -n 4 ./point2point < arg.txt
mpirun -n 8 ./point2point < arg.txt
echo '1024 1024 1024' > arg.txt
mpirun -n 1 ./point2point < arg.txt
mpirun -n 2 ./point2point < arg.txt
mpirun -n 4 ./point2point < arg.txt
mpirun -n 8 ./point2point < arg.txt
echo '2048 2048 2048' > arg.txt
mpirun -n 1 ./point2point < arg.txt
mpirun -n 2 ./point2point < arg.txt
mpirun -n 4 ./point2point < arg.txt
mpirun -n 8 ./point2point < arg.txt