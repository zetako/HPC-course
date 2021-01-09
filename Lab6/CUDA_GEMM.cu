#include <iostream>
#include <sstream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <chrono>
#include <cuda.h>
#include <cuda_runtime.h>
#include "check.hpp"

using namespace std::chrono;

__global__ 
void mat_mul(double *A,double *B,double *C,int *p_M,int *p_N,int *p_K)
{
    //__syncthreads();
    int M,N,K;
    M=*p_M;
    N=*p_N;
    K=*p_K;
    // get self idx
    int i=blockIdx.x*blockDim.x+threadIdx.x; 
    int j=blockIdx.y*blockDim.y+threadIdx.y;
    if (i>=M||j>=K) return;
    

    // cal
    C[i*K+j]=0.0;
    for (int n=0;n<N;n++) C[i*K+j]+=A[i*N+n]*B[n*K+j];
}

int main(int argc, char *argv[])
{
    double *A,*B,*C;
    double *device_A,*device_B,*device_C;

    // input settings
    int blockNum;
    std::cout<<"input block number: ";
    std::cin>>blockNum;
    std::cout<<std::endl;

    int M,N,K;
    std::cout<<"input M,N,K: ";
    std::cin>>M>>N>>K;
    std::cout<<std::endl;
    
    // host's space
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

    // device's space
    cudaMalloc((void **)&device_A,sizeof(double)*M*N);
    cudaMalloc((void **)&device_B,sizeof(double)*N*K);
    cudaMalloc((void **)&device_C,sizeof(double)*M*K);
    int *device_M,*device_N,*device_K;
    cudaMalloc((void **)&device_M,sizeof(int));
    cudaMalloc((void **)&device_N,sizeof(int));
    cudaMalloc((void **)&device_K,sizeof(int));

    
    auto start=system_clock::now();
    // pass value
    cudaMemcpy(device_A,A,sizeof(double)*M*N,cudaMemcpyHostToDevice);
    cudaMemcpy(device_B,B,sizeof(double)*N*K,cudaMemcpyHostToDevice);
    cudaMemcpy(device_M,&M,sizeof(int),cudaMemcpyHostToDevice);
    cudaMemcpy(device_N,&N,sizeof(int),cudaMemcpyHostToDevice);
    cudaMemcpy(device_K,&K,sizeof(int),cudaMemcpyHostToDevice);

    dim3 grid(M/blockNum+1,K);
    dim3 block(blockNum);
    mat_mul<<<grid,block>>>(device_A,device_B,device_C,device_M,device_N,device_K);

    cudaMemcpy(C,device_C,sizeof(double)*M*K,cudaMemcpyDeviceToHost);

    auto end=system_clock::now();
    auto duration=duration_cast<microseconds>(end-start);
    std::stringstream ss;
    ss<<"echo '";
    ss<<"M="<<M<<",N="<<N<<",K="<<K<<"\t";
    ss<<"blocks="<<blockNum<<",time="<<std::fixed<<double(duration.count())<<"us";
    ss<<"' >> CUDA_GEMM.log";
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

    delete[] A;
    delete[] B;
    delete[] C;

}