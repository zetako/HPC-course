#include "matrix.hpp"
#include "pth4omp.hpp"
#include <iostream>
#include <chrono>
#include <sstream>
#include <string>
#include <cstdlib>

using namespace std;
using namespace chrono;

struct itorArg
{
    matrix *A;
    matrix *B;
    matrix *C;
    int M;
    int N;
    int K;
};

void itorWork(void *arg,int i)
{
    itorArg *pack=(itorArg *)arg;
    int M=pack->M;
    int N=pack->N;
    int K=pack->K;

    for (int j=0;j<K;j++)
    {
        int sum=0;
        for (int k=0;k<N;k++) sum+=(pack->A)->content[i*N+k]*(pack->B)->content[k*K+j];
        (pack->C)->content[i*K+j]=sum;
    }

    return;
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

    #pragma omp parallel for num_threads(threadNum)
    for (int i=0;i<M;i++)
        for (int j=0;j<K;j++)
        {
            int sum=0;
            for (int k=0;k<N;k++) sum+=A.content[i*N+k]*B.content[k*K+j];
            C.content[i*K+j]=sum;
        }
    C.generated=true;
    itorArg arg{&A,&B,&C,M,N,K};
    parallel_for(0,M,1,itorWork,(void *)&arg,threadNum);

    auto end=system_clock::now();

    C.print();
    auto duration=duration_cast<microseconds>(end-start);
    
    stringstream ss;
    ss<<"echo '";
    ss<<"M="<<M<<",N="<<N<<",K="<<K<<"\t";
    ss<<"threads="<<threadNum<<",time="<<fixed<<double(duration.count())<<"us";
    ss<<"' >> pth4omp_GEMM.log";
    string cmd=ss.str();
    cout<<cmd<<endl;
    system(cmd.c_str());

    return 0;
}