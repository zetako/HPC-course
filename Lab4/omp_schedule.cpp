#include "matrix.hpp"
#include <iostream>
#include <omp.h>
#include <chrono>
#include <sstream>
#include <string>
#include <cstdlib>

using namespace std;
using namespace chrono;

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

    system_clock::time_point start[3];
    system_clock::time_point end[3];

    //default schedule
    start[0]=system_clock::now();
    #pragma omp parallel for num_threads(threadNum)
    for (int i=0;i<M;i++)
        for (int j=0;j<K;j++)
        {
            int sum=0;
            for (int k=0;k<N;k++) sum+=A.content[i*N+k]*B.content[k*K+j];
            C.content[i*K+j]=sum;
        }
    C.generated=true;
    end[0]=system_clock::now();

    //static schedule
    start[1]=system_clock::now();
    #pragma omp parallel for num_threads(threadNum) schedule(static,1)
    for (int i=0;i<M;i++)
        for (int j=0;j<K;j++)
        {
            int sum=0;
            for (int k=0;k<N;k++) sum+=A.content[i*N+k]*B.content[k*K+j];
            C.content[i*K+j]=sum;
        }
    C.generated=true;
    end[1]=system_clock::now();

    //dynamic schedule
    start[2]=system_clock::now();
    #pragma omp parallel for num_threads(threadNum) schedule(dynamic,1)
    for (int i=0;i<M;i++)
        for (int j=0;j<K;j++)
        {
            int sum=0;
            for (int k=0;k<N;k++) sum+=A.content[i*N+k]*B.content[k*K+j];
            C.content[i*K+j]=sum;
        }
    C.generated=true;
    end[2]=system_clock::now();

    C.print();
    microseconds duration[3];
    duration[0]=duration_cast<microseconds>(end[0]-start[0]);
    duration[1]=duration_cast<microseconds>(end[1]-start[1]);
    duration[2]=duration_cast<microseconds>(end[2]-start[2]);
    
    stringstream ss;
    ss<<"echo '";
    ss<<"M="<<M<<",N="<<N<<",K="<<K<<"\t";
    ss<<"threads="<<threadNum<<",time=";
    ss<<fixed<<double(duration[0].count())<<'/';
    ss<<fixed<<double(duration[1].count())<<'/';
    ss<<fixed<<double(duration[2].count())<<"us";
    ss<<"' >> omp_schedule.log";
    string cmd=ss.str();
    cout<<cmd<<endl;
    system(cmd.c_str());

    return 0;
}