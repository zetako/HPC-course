#include <cstdlib>
#include <sys/time.h>
#include <cuda_runtime.h>
#include <cublas_v2.h>

#include <iostream>
#include <sstream>
#include <string>
#include <chrono>
#include "check.hpp"

using namespace std::chrono;

int main(int argc, char const *argv[])
{
    // input setting
    int M,N,K;
    std::cout<<"input M,N,K: ";
    std::cin>>M>>N>>K;
    std::cout<<std::endl;
    
    // host's space
    double *A,*B,*C,alpha=1.0,beta=0.0;
    A=new double[M*N];
    B=new double[N*K];
    C=new double[M*K];

    // initialize
    srand(time(NULL));
    int tmp=M*N;
    for (int i=0;i<tmp;i++) A[i]=(double)(rand()%6400)/100;
    tmp=N*K;
    for (int i=0;i<tmp;i++) B[i]=(double)(rand()%6400)/100;
    bzero(C,M*K*sizeof(double));

    for (int i=0;i<M;i++)
    {
        for (int j=0;j<N;j++)
        {
            std::cout<<A[i*N+j]<<' ';
        }
        std::cout<<std::endl;
    }
    std::cout<<"=================================="<<std::endl;
    for (int i=0;i<N;i++)
    {
        for (int j=0;j<K;j++)
        {
            std::cout<<B[i*K+j]<<' ';
        }
        std::cout<<std::endl;
    }
    std::cout<<"=================================="<<std::endl;
    
    cublasHandle_t handle;
    cublasCreate(&handle);
    double *device_A, *device_B, *device_C;
    // device's space
    cudaMalloc((void **)&device_A,sizeof(double)*M*N);
    cudaMalloc((void **)&device_B,sizeof(double)*N*K);
    cudaMalloc((void **)&device_C,sizeof(double)*M*K);
    // pass value

    const double *device_A_const = device_A;
    const double *device_B_const = device_B;

    auto start=system_clock::now();
    cudaMemcpy(device_A,A,sizeof(double)*M*N,cudaMemcpyHostToDevice);
    cudaMemcpy(device_B,B,sizeof(double)*N*K,cudaMemcpyHostToDevice);
    cublasDgemm(handle,CUBLAS_OP_N,CUBLAS_OP_N,K,M,N,&alpha,
        device_B_const,K,
        device_A_const,N,&beta,
        device_C,K);
    cudaMemcpy(C,device_C,M*K*sizeof(double),cudaMemcpyDeviceToHost);
    auto end=system_clock::now();
    auto duration=duration_cast<microseconds>(end-start);
    std::stringstream ss;
    ss<<"echo '";
    ss<<"M="<<M<<",N="<<N<<",K="<<K<<"\t";
    ss<<"time="<<std::fixed<<double(duration.count())<<"us";
    ss<<"' >> BLAS_GEMM.log";
    std::string cmd=ss.str();
    std::cout<<cmd<<std::endl;
    system(cmd.c_str());

    for (int i=0;i<M;i++)
    {
        for (int j=0;j<K;j++)
        {
            std::cout<<C[i*K+j]<<' ';
        }
        std::cout<<std::endl;
    }

    checkMatrix(A,B,C,M,N,K);
    
    cublasDestroy(handle);
    return 0;
}

