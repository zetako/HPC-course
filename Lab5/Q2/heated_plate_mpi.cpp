#include <mpi.h>
#include <iostream>
#include <iomanip>
#include "heated_plate_mpi_worker.hpp"
using namespace std;

int main(int argc,char const *argv[])
{
    MPI_Init(NULL,NULL);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    /* commit a struct */
    MPI_Datatype blockType;
    int arr_len[2]={2,2};
    MPI_Aint arr_dis[2]={0,0};
    arr_dis[1]=2*sizeof(MPI_INT);
    MPI_Datatype arr_type[2]={MPI_INT,MPI_INT};
    MPI_Type_create_struct(2,arr_len,arr_dis,arr_type,&blockType);
    MPI_Type_commit(&blockType);
    
    /* SPMD */
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    if (rank==0)
    {
        /* init */
        int M,N;
        cout<<"input M, N:";
        cin>>M>>N;
        double *ori,*cur;
        initPlate(ori,cur,M,N);
        MPI_Bcast(&M,1,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Bcast(&N,1,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Bcast(ori,M*N,MPI_DOUBLE,0,MPI_COMM_WORLD);

        /* calculate blocks */
        //it's safe to consider size is a square
        block tmp;
        for (int i=0;i<size;i++)
        {
            tmp=getBlock(M,N,size,i);
            MPI_Send(&tmp,1,blockType,i,TAG_blockInfo,MPI_COMM_WORLD);
        }
        block myBlock=getBlock(M,N,size,0);
        

        /* loop */
        int bufSize=getBlockBufSize(M,N,size);
        char *blockBuf;
        initBlockBuf(blockBuf,size);
        int itorCount=0;
        int itorPrint=1;
        int flag=1;

        double diffOther,diffMine,diff;
        while(true)
        {
            /* backup old plate */
            backupPlate(ori,cur,M,N);

            /* itor & update */
            itorBlock(cur,myBlock);
            itorLeft(cur,M,N,size);
            for (int i=0;i<size;i++)
            {
                MPI_Recv(blockBuf,bufSize,MPI_PACKED,i,TAG_itor(itorCount),MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                updatePlate(cur,M,N,size,i,blockBuf);
            }
            MPI_Bcast(cur,M*N,MPI_DOUBLE,0,MPI_COMM_WORLD);

            /* calculate diff */
            diffMine=diffBlock(ori,cur,myBlock);
            diff=diffLeft(ori,cur,M,N,size);
            MPI_Reduce(&diffMine,&diffOther,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);
            diff=max<double>(diffOther,diff);

            /* print log & exit */
            itorCount++;
            if (diff<epsilon)
            {
                cout<<endl;
                cout<<setw(8)<<setfill(' ')<<itorCount<<'  '<<diff<<endl;
                flag=0;
                MPI_Bcast(&flag,1,MPI_INT,0,MPI_COMM_WORLD);
                break;
            }
            if (itorCount==itorPrint)
            {
                cout<<setw(8)<<setfill(' ')<<itorCount<<'  '<<diff<<endl;
                itorPrint*=2;
            }
            MPI_Bcast(&flag,1,MPI_INT,0,MPI_COMM_WORLD);
        }
        destoryPlate(ori,cur,M,N);
    }
    else
    {
        /* init env */
        int M,N;
        MPI_Bcast(&M,1,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Bcast(&N,1,MPI_INT,0,MPI_COMM_WORLD);
        double *ori,*cur;
        ori=new double[M*N];
        cur=new double[M*N];
        MPI_Bcast(cur,M*N,MPI_DOUBLE,0,MPI_COMM_WORLD);

        block myBlock;
        MPI_Recv(&myBlock,1,blockType,0,TAG_blockInfo,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

        /* loop */
        int bufSize=getBlockBufSize(M,N,size);
        char *blockBuf;
        initBlockBuf(blockBuf,size);
        int itorCount=0;
        double diff;
        int flag=1;

        while (flag)
        {
            /* backup old plate */
            backupPlate(ori,cur,M,N);

            /* itor & update */
            itorBlock(cur,myBlock);
            packBlockBuf(blockBuf,cur,myBlock);
            MPI_Send(blockBuf,bufSize,MPI_PACKED,0,TAG_itor(itorCount),MPI_COMM_WORLD);
            MPI_Bcast(cur,M*N,MPI_DOUBLE,0,MPI_COMM_WORLD);

            /* calculate diff */
            diff=diffBlock(ori,cur,myBlock);
            MPI_Reduce(&diff,NULL,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);
 
            /* recv continue */
            MPI_Bcast(&flag,1,MPI_INT,0,MPI_COMM_WORLD);
        }
        destoryPlate(ori,cur,M,N);
    }

    MPI_Finalize();
    return 0;
}