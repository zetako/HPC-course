#include "heated_plate_mpi_worker.hpp"

void initPlate(double *&ori,double *&cur,int M,int N)
{
    /* get space */
    ori=new double[M*N];
    cur=new double[M*N];

    /* init border */
    double mean=0.0;
    for (int i=1;i<M-1;i++)
    {
        cur[i*N]=initLeft;
        mean+=initLeft;
        cur[i*N+N-1]=initRight;
        mean+=initRight;
    }
    for (int j=0;j<N;j++)
    {
        cur[(M-1)*N+j]=initBottom;
        mean+=initBottom;
        cur[j]=initTop;
        mean+=initTop;
    }
    mean=mean/(double)(2*M+2*N-4);
    for (int i=1;i<M-1;i++)
        for (int j=1;j<M-1;j++) cur[i*N+j]=mean;
    
    return;
}
void destoryPlate(double *&ori,double *&cur,int M,int N);
void backupPlate(double *&ori,double *cur,int M,int N);
block getBlock(int M,int N,int size,int rank);
void itorBlock(double *plate,block b);
void itorLeft(double *plate,int M,int N,int size);
int getBlockBufSize(int M,int N,int size);
void initBlockBuf(char *&buf,int len);
void packBlockBuf(char *buf,double *plate,block b);
void updatePlate(double *plate,int M,int N,int size,int rank,char *buf);
double diffBlock(double *ori,double *cur,block b);
double diffLeft(double *ori,double *cur,int M,int N,int size);