#include "check.hpp"
#include <iostream>

inline bool isSame(double d1,double d2)
{
    double delta=d1-d2;
    if (delta<0) delta=-delta;
    return delta<epsilon;
}
int checkMatrix(double *A,double*B,double *C,int M,int N,int K)
{
    if (M>checkBorder||N>checkBorder||K>checkBorder)
    {
        std::cout<<"matrix size too much, check cancel"<<std::endl;
        return -1;
    }

    double *checkMat=new double[M*K];
    int count=0;

    for (int i=0;i<M;i++)
    {
        for (int j=0;j<K;j++)
        {
            checkMat[i*K+j]=0;
            for (int n=0;n<N;n++) checkMat[i*K+j]+=A[i*N+n]*B[n*K+j];
            if (isSame(checkMat[i*K+j],C[i*K+j])) continue;
            count++;
        }
    }

    std::cout<<"at epsilon of "<<epsilon;
    std::cout<<",there is "<<count<<" different result"<<std::endl;
    return count;
}