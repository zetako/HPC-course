#!/bin/bash

make point2point
echo '512 512 512' > arg.txt
mpirun --oversubscribe -n 1 ./point2point < arg.txt
mpirun --oversubscribe -n 2 ./point2point < arg.txt
mpirun --oversubscribe -n 4 ./point2point < arg.txt
mpirun --oversubscribe -n 8 ./point2point < arg.txt
echo '1024 1024 1024' > arg.txt
mpirun --oversubscribe -n 1 ./point2point < arg.txt
mpirun --oversubscribe -n 2 ./point2point < arg.txt
mpirun --oversubscribe -n 4 ./point2point < arg.txt
mpirun --oversubscribe -n 8 ./point2point < arg.txt
echo '2048 2048 2048' > arg.txt
mpirun --oversubscribe -n 1 ./point2point < arg.txt
mpirun --oversubscribe -n 2 ./point2point < arg.txt
mpirun --oversubscribe -n 4 ./point2point < arg.txt
mpirun --oversubscribe -n 8 ./point2point < arg.txt