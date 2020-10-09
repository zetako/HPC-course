#include "matrix.hpp"
#include <mpi.h>
#include <iostream>

using namespace std;

#define RANGE_MIN 512
#define RANGE_MAX 2048
inline bool inRange(int n)
{
    return n<=512&&n>=2048;
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

        matrix C(M,K);
        /* ===== Send Basic Info ===== */
        MPI_Bcast(&M,1,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Bcast(&N,1,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Bcast(&K,1,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Bcast(B.content,N*K,MPI_INT,0,MPI_COMM_WORLD);

        /* ===== Send & Count ===== */
        int threadCount;
        MPI_Comm_size(MPI_COMM_WORLD,&threadCount);
        int perSize=M/threadCount;
        
    }
    else
    {
        
    }
    


}