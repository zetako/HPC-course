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

    auto end=system_clock::now();

    C.print();
    auto duration=duration_cast<microseconds>(end-start);
    
    stringstream ss;
    ss<<"echo '";
    ss<<"M="<<M<<",N="<<N<<",K="<<K<<"\t";
    ss<<"threads="<<threadNum<<",time="<<fixed<<double(duration.count())<<"us";
    ss<<"' >> omp_GEMM.log";
    string cmd=ss.str();
    cout<<cmd<<endl;
    system(cmd.c_str());

    return 0;
}