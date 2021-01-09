#include "heated_plate_work_func.hpp"
#include <pthread.h>
#include <math.h>
#include <iostream>

double mean;
double diff;
int M=500;
int N=500;
double **u;
double **w;

namespace zetako{

    void getSpace()
    {
        w=new double*[M];
        u=new double*[M];
        for (int i=0;i<M;i++)
        {
            w[i]=new double[N];
            u[i]=new double[N];
        }
    }
    void releaseSpace()
    {

        for (int i=0;i<M;i++)
        {
            delete[] w[i];
            delete[] u[i];
        }
        delete[] w;
        delete[] u;
    }

    void setBoundary(void *arg,int itor)
    {
        _boundary_ type=*(_boundary_ *)arg;
        switch (type)
        {
        case top:
            w[0][itor]=0.0;
            break;
        case bottom:
            w[M-1][itor]=100.0;
            break;
        case left:
            w[itor][0]=100.0;
            break;
        case right:
            w[itor][N-1]=100.0;
            break;
        default:
            break;
        }
        return;
    }

    void initInter(void *arg,int itor)
    {
        for (int j=1;j<N-1;j++)
        {
            w[itor][j]=mean;
        }
        return;
    }

    void saveOld(void *arg,int itor)
    {
        for (int j=0;j<N;j++)
        {
            u[itor][j]=w[itor][j];
        }
        return;
    }

    void setNew(void *arg,int itor)
    {
        for (int j=1;j<N-1;j++)
        {
            w[itor][j]=(u[itor-1][j]+u[itor+1][j]+u[itor][j-1]+u[itor][j+1])/4.0;
        }
        return;
    }

    void updateDiff(void *arg,int itor)
    {
        pthread_mutex_t *diffLock=(pthread_mutex_t *)arg;
        double my_diff=0.0;

        for (int j=1;j<N-1;j++)
        {
            if (my_diff<fabs(w[itor][j]-u[itor][j]))
            {
                my_diff=fabs(w[itor][j]-u[itor][j]);
            }
        }

        pthread_mutex_lock(diffLock);
        if (diff<my_diff)
        {
            diff=my_diff;
        }
        pthread_mutex_unlock(diffLock);
    }

}