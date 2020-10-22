#include <pthread.h>
#include <iostream>
#include <random>
#include <chrono>

using namespace std;
using namespace chrono;

#define ARRAY_SIZE 1000

int global_index=0;
pthread_mutex_t index_lock;

void *threadWork(void *arg)
{
    int *sum=new int;
    *sum=0;
    int tmp;
    int *a=(int *)arg;
    bool flag=true;
    while (flag)
    {
        pthread_mutex_lock(&index_lock);
        if (global_index>=ARRAY_SIZE) flag=false;
        else tmp=a[global_index];
        pthread_mutex_unlock(&index_lock);
        *sum+=tmp;
    }
    pthread_exit((void *)sum);
    return (void *)sum;
}

void genArray(int *a)
{
    static random_device rd;
    static default_random_engine e(rd());
    static uniform_int_distribution<int> u(0,2048);

    for (int i=0;i<ARRAY_SIZE;i++)
        a[i]=u(e);
}

int main()
{
    int a[ARRAY_SIZE];
    genArray(a);
    int threadNum;
    cout<<"Thread Numbers:";
    cin>>threadNum;
    cout<<"..."<<endl;

    auto start=system_clock::now();

    pthread_t *pth=new pthread_t[threadNum];
    pthread_mutex_init(&index_lock,0);
    for (int i=1;i<threadNum;i++)
        pthread_create(pth+i,NULL,threadWork,(void *)a);
    int sum=0;
    int tmp;
    bool flag=true;
    while (flag)
    {
        pthread_mutex_lock(&index_lock);
        if (global_index>=ARRAY_SIZE) flag=false;
        else tmp=a[global_index++];
        pthread_mutex_unlock(&index_lock);
        sum+=tmp;
    }

    int *local_sum;
    for (int i=1;i<threadNum;i++)
    {
        pthread_join(pth[i],(void **)&local_sum);
        sum+=*local_sum;
        delete local_sum;
    }

    pthread_mutex_destroy(&index_lock);

    cout<<"sum="<<sum<<endl;
    auto end=system_clock::now();
    auto duration=duration_cast<microseconds>(end-start);
    cout<<"threads="<<threadNum<<",time="<<double(duration.count())<<"ms."<<endl;
    return 0;
    
}