#include "matrix.hpp"
#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>

using namespace std;

#define RANGE_MIN 512
#define RANGE_MAX 2048
inline bool inRange(int n)
{
    return n>=512&&n<=2048;
}


int main(int argc,char const *argv[])
{
/* 
** Thread 0 : perform I/O, send/recv data to other thread
** All Threads : count per line, send back to main thread
** P.S. thread 0 also join calculation
*/

    MPI_Init(NULL,NULL);
    int id;
    MPI_Comm_rank(MPI_COMM_WORLD,&id);
    if (id==0)
    {
        /* ===== Get Data & Gen Matrix ===== */
        cout<<"Please Input M,N,K:";
        int M,N,K;
        cin>>M>>N>>K;
        bool exitFlag=false;
        if (!inRange(M)) cout<<"invaild M"<<endl;
        else if (!inRange(N)) cout<<"invaild N"<<endl;
        else if (!inRange(K)) cout<<"invaild K"<<endl;
        else exitFlag=true;
        if (!exitFlag) return -1;

        matrix A(M,N);
        matrix B(N,K);
        A.gen();
        B.gen();
        A.print();
        B.print();

        matrix C(M,K);

        /* ===== Start Count Time ===== */
        double start=MPI_Wtime();

        /* ===== Send Basic Info ===== */
        MPI_Bcast(&M,1,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Bcast(&N,1,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Bcast(&K,1,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Bcast(B.content,N*K,MPI_INT,0,MPI_COMM_WORLD);

        /* ===== Send to Target ===== */
        int threadCount;
        MPI_Comm_size(MPI_COMM_WORLD,&threadCount);
        int perSize=M/threadCount;
        for (int i=1;i<threadCount;i++) MPI_Send(B.content+perSize*i*N,perSize*N,MPI_INT,i,1,MPI_COMM_WORLD);

        /* ===== Count ===== */
        for (int i=0;i<perSize;i++)
        {
            for (int j=0;j<K;j++)
            {
                int sum=0;
                for (int k=0;k<N;k++) sum+=(A.content[i*N+k])*(B.content[k*K+j]);
                C.content[i*K+j]=sum;
            }
        }
        for (int i=perSize*threadCount;i<M;i++)
        {
            for (int j=0;j<K;j++)
            {
                int sum=0;
                for (int k=0;k<N;k++) sum+=(A.content[i*N+k])*(B.content[k*K+j]);
                C.content[i*K+j]=sum;
            }
        }

        /* ===== Recv From Others ===== */
        for (int i=1;i<threadCount;i++) MPI_Recv(C.content+perSize*i*K,perSize*K,MPI_INT,i,2,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

        /* ===== active C ===== */
        C.generated=true;

        /* ===== End Count ===== */
        double end=MPI_Wtime();
        double durTime=end-start;
        stringstream ss;
        ss<<"echo '";
        ss<<"M="<<M<<",N="<<N<<",K="<<K<<"\t";
        ss<<"threads="<<threadCount<<",time="<<durTime<<"s";
        ss<<"' >> point2point.log";
        string cmd=ss.str();
        cout<<cmd<<endl;
        system(cmd.c_str());

        C.print();
    }
    else
    {
        /* ===== Init ===== */
        int M,N,K;
        
        /* ===== Recv From Main Thread ===== */
        MPI_Bcast(&M,1,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Bcast(&N,1,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Bcast(&K,1,MPI_INT,0,MPI_COMM_WORLD);
        matrix B(N,K);
        MPI_Bcast(B.content,N*K,MPI_INT,0,MPI_COMM_WORLD);

        int threadCount;
        MPI_Comm_size(MPI_COMM_WORLD,&threadCount);
        int perSize=M/threadCount;

        int *recvbuff=new int[perSize*N];
        int *sendbuff=new int[perSize*K];
        MPI_Recv(recvbuff,perSize*N,MPI_INT,0,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

        /* ===== Count ===== */
        for (int i=0;i<perSize;i++)
        {
            for (int j=0;j<K;j++)
            {
                int sum=0;
                for (int k=0;k<N;k++) sum+=(recvbuff[i*N+k])*(B.content[k*K+j]);
                sendbuff[i*K+j]=sum;
            }
        }

        /* ===== Send Back & Free ===== */
        MPI_Send(sendbuff,perSize*K,MPI_INT,0,2,MPI_COMM_WORLD);
        delete[] recvbuff;
        delete[] sendbuff;
    }
    MPI_Finalize();
    
    return 0;
}