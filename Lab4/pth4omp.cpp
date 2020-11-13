#include "pth4omp.hpp"
#include <pthread.h>

struct argPak
{
    int startItor;
    int repeatTime;
    void (*func)(void *,int);
    void *innerPak;
};
void *workLoadStart(void *arg)
{
    argPak *opt=(argPak *)arg;
    for (int i=0;i<opt->repeatTime;i++)
        opt->func(opt->innerPak,(opt->startItor)+i);
    pthread_exit(NULL);
    return NULL;
}

void pth_omp_for(int start,int end,
                int inc,void (*func)(void *,int),
                void *arg,int threadNum)
{
    int workLoad=(end-start+1)/inc;
    int perThread=workLoad/threadNum;

    pthread_t *pth=new pthread_t[threadNum];
    argPak *pack=new argPak[threadNum];

    //other threads
    for (int i=1;i<threadNum;i++)
    {
        pack[i].func=func;
        pack[i].innerPak=arg;
        pack[i].repeatTime=perThread;
        pack[i].startItor=i*perThread;

        pthread_create(pth+i,NULL,workLoadStart,(void *)(pack+i));
    }

    //thread #0 do it's own work
    for (int i=0;i<perThread;i++) func(arg,i);
    //thread #0 do the rest of work
    for (int i=perThread*threadNum;i<workLoad;i++) func(arg,i);

    //join to sync
    for (int i=1;i<threadNum;i++) pthread_join(pth[i],NULL);

    delete[] pth;
    delete[] pack;
    return;
}