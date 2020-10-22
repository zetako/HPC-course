#include <iostream>
#include <random>
#include <pthread.h>

using namespace std;

typedef pair<double,double> point;

inline point gen()
{
    static random_device rd;
    static default_random_engine e(rd());
    static uniform_real_distribution<double> u(0.0,1.0);
    return point(u(e),u(e));
}
inline bool isIn(point p)
{
    return (p.first*p.first>p.second);
}
void *threadWork(void *arg)
{
    int size=*(int *)arg;
    int *sum=new int;
    for (int i=0;i<size;i++)
        if (isIn(gen())) *sum++;
    pthread_exit(sum);
    return (void *)sum;
}

int main()
{
    int testCase,threadNum;
    cout<<"test cases:";
    cin>>testCase;
    cout<<"..."<<endl<<"thread number:";
    cin>>threadNum;
    cout<<"..."<<endl;
    int perThread=threadNum/testCase;

    pthread_t *pth=new pthread_t[threadNum];
    for (int i=1;i<threadNum;i++)
        pthread_create(pth+i,NULL,threadWork,(void *)&perThread);
    int sum=0;
    for (int i=0;i<perThread;i++)
        if (isIn(gen())) sum++;
    for (int i=perThread*threadNum;i<testCase;i++)
        if (isIn(gen())) sum++;
    for (int i=0;i<perThread;i++)
    {
        int *local_sum;
        pthread_join(pth[i],(void **)&local_sum);
        sum+=*local_sum;
        delete local_sum;
    }

    double area=(double)sum/(double)testCase;
    cout<<"simulation of "<<testCase<<" times, area is "<<area<<endl;
}
