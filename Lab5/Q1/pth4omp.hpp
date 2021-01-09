#pragma once
#ifndef PTH_FOR_OMP
#define PTH_FOR_OMP 1

#define parallel_for pth_omp_for
void pth_omp_for(int start,int end,int inc,void (*func)(void *,int),void *arg,int threadNum);

#endif