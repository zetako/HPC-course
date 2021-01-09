#pragma once
#ifndef CHECK_HPP
#define CHECK_HPP 1

#define epsilon 0.0001
#define checkBorder 512

inline bool isSame(double d1,double d2);

int checkMatrix(double *A,double*B,double *C,int M,int N,int K);

#endif