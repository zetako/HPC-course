#include "matrix.hpp"
#include <iostream>
#include <pthread.h>
#include <chrono>
#include <sstream>
#include <string>
#include <cstdlib>

using namespace std;
using namespace chrono;

struct threadWorkArg
{
    int *lines;     //in    //lines to count
    int lineNum;    //in    //lines to count
    int *B;         //in    //matrix B
    int *C;         //out   //matrix C
    int M,N,K;      //in    //size
};

void *threadWork(void *arg)
{
    threadWorkArg *pack=(threadWorkArg*)arg;
    int M=pack->M;
    int N=pack->N;
    int K=pack->K;

    /* ===== Count ===== */
    for (int i=0;i<pack->lineNum;i++)
    {
        for (int j=0;j<K;j++)
        {
            int sum=0;
            for (int k=0;k<N;k++) sum+=(pack->lines[i*N+k])*(pack->B[k*K+j]);
            pack->C[i*K+j]=sum;
        }
    }
    
    pthread_exit(NULL);
    return NULL;
}
int main()
{
    int threadNum;
    cout<<"Input thread number:";
    cin>>threadNum;
    cout<<endl;

    cout<<"Input M,N,K:";
    int M,N,K;
    cin>>M>>N>>K;
    cout<<endl;

    matrix A(M,N);
    matrix B(N,K);
    A.gen();
    B.gen();
    A.print();
    B.print();
    matrix C(M,K);

    auto start=system_clock::now();

    int lineNum=M/threadNum;
    threadWorkArg *args=new threadWorkArg[threadNum];
    pthread_t *pth=new pthread_t[threadNum];
    for (int i=1;i<threadNum;i++)
    {
        args[i].lines=A.content+i*lineNum*N;
        args[i].lineNum=lineNum;
        args[i].B=B.content;
        args[i].C=C.content+i*lineNum*K;
        args[i].M=M;
        args[i].N=N;
        args[i].K=K;

        pthread_create(pth+i,NULL,threadWork,(void *)(args+i));
    }

    for (int i=0;i<lineNum;i++)
    {
        for (int j=0;j<K;j++)
        {
            int sum=0;
            for (int k=0;k<N;k++) sum+=(A.content[i*N+k])*(B.content[k*K+j]);
            C.content[i*K+j]=sum;
        }
    }
    for (int i=lineNum*threadNum;i<M;i++)
    {
        for (int j=0;j<K;j++)
        {
            int sum=0;
            for (int k=0;k<N;k++) sum+=(A.content[i*N+k])*(B.content[k*K+j]);
            C.content[i*K+j]=sum;
        }
    }

    for (int i=1;i<threadNum;i++) pthread_join(pth[i],NULL);
    delete[] args;
    delete[] pth;
    C.generated=true;

    auto end=system_clock::now();

    C.print();
    auto duration=duration_cast<microseconds>(end-start);
    
    stringstream ss;
    ss<<"echo '";
    ss<<"M="<<M<<",N="<<N<<",K="<<K<<"\t";
    ss<<"threads="<<threadNum<<",time="<<fixed<<double(duration.count())<<"ms";
    ss<<"' >> pth_GEMM.log";
    string cmd=ss.str();
    cout<<cmd<<endl;
    system(cmd.c_str());

    return 0;
}