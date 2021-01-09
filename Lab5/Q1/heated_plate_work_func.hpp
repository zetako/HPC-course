#pragma once
#ifndef HEATED_PLATE_WORKER
#define HEATED_PLATE_WORKER 1

extern double mean;
extern double diff;
extern int M;
extern int N;
extern double **u;
extern double **w;

namespace zetako{

    /* basic setting */
    void getSpace();
    void releaseSpace();

    /* set boundary */
    enum _boundary_
    {
        top=0,
        bottom=1,
        left=2,
        right=3,
    };
    void setBoundary(void *arg,int itor);

    /* initialize interior */
    void initInter(void *arg,int itor);

    /* save old */
    void saveOld(void *arg,int itor);

    /* determine new */
    void setNew(void *arg,int itor);

    /* update diff */
    void updateDiff(void *arg,int itor);

    }

#endif