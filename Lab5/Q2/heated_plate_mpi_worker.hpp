#pragma once
#ifndef HEATED_PLATE_MPI_WORKER
#define HEATED_PLATE_MPI_WORKER

const double epsilon=0.001;
const double initTemp=100.0;
#define initLeft initTemp
#define initRight initTemp
#define initTop 0
#define initBottom initTemp


#define at(m,x,y) m[x*N+y]

#define TAG_blockInfo 0
#define TAG_itor(n) (1+(n))

struct block
{
    int x_start,x_end;
    int y_start,y_end;
};

void initPlate(double *&ori,double *&cur,int M,int N);
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


#endif