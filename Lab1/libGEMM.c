#include "libGEMM.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <mpi.h>
#include <omp.h>

bool basic_multi(matrix *a,matrix *b,matrix *c)
{
    /* validation */
    if (c->dfn==true) return false;
    if (a->dfn==false||b->dfn==false) return false;
    if (a->y!=b->x) return false;

    /* computation */
    //C_{m,n}=\sum_{n=1}^N A_{m,n}B_{n,k}
    int x=a->x,y=b->y;
    c->x=x;
    c->y=y;
    c->content=(int **)malloc(sizeof(int *)*x);
    for (int i=0;i<x;i++)
    {
        c->content[i]=(int *)malloc(sizeof(int)*y);
        for (int j=0;j<y;j++)
        {
            int sum=0;
            for (int k=0;k<a->y;k++) sum+=(a->content[i][k])*(b->content[k][j]);
            c->content[i][j]=sum;
        }
    }
    c->dfn=true;
    return true;
}
bool omp_multi(matrix *a,matrix *b,matrix *c)
{
    /* validation */
    if (c->dfn==true) return false;
    if (a->dfn==false||b->dfn==false) return false;
    if (a->y!=b->x) return false;

    /* computation */
    //C_{m,n}=\sum_{n=1}^N A_{m,n}B_{n,k}
    int x=a->x,y=b->y;
    c->x=x;
    c->y=y;
    c->content=(int **)malloc(sizeof(int *)*x);
    #pragma omp parallel for
    for (int i=0;i<x;i++)
    {
        c->content[i]=(int *)malloc(sizeof(int)*y);
        for (int j=0;j<y;j++)
        {
            int sum=0;
            for (int k=0;k<a->y;k++) sum+=(a->content[i][k])*(b->content[k][j]);
            c->content[i][j]=sum;
        }
    }
    c->dfn=true;
    return true;
}
bool strassen_multi(matrix *a,matrix *b,matrix *c)
{
    /* check validation */
    if (a->dfn==false||b->dfn==false||c->dfn==true) return false;
    if (a->x!=b->x||a->y!=b->y) return false;
    /* check size */
    if (a->y!=b->x) return false;
    
    /* implement when not power of 2 */
    int n=a->x;
    int m=a->y;
    matrix *new_a,*new_b;
    if (n&(n-1)!=0||m&(m-1)!=0) new_a=implement_mat(a);
    else new_a=a;
    n=b->x;
    m=b->y;
    if (n&(n-1)!=0||m&(m-1)!=0) new_b=implement_mat(b);
    else new_b=b;

    matrix *c;
    /* call the main recursion func */
    bool status=strassen_mul(pack_mat(new_a,full),pack_mat(new_b,full),c);
    return status;
}
bool add_mat(matrix *a,matrix *b,matrix *c)
{
    if (a->dfn==false||b->dfn==false||c->dfn==true) return false;
    if (a->x!=b->x||a->y!=b->y) return false;

    c->x=a->x;
    c->y=a->y;
    int x=a->x;
    int y=a->y;
    c->content=(int **)malloc(sizeof(int *)*x);
    for (int i=0;i<x;i++)
    {
        c->content[i]=(int *)malloc(sizeof(int)*y);
        for (int j=0;j<y;j++)
            c->content[i][j]=a->content[i][j]+b->content[i][j];
    }
    c->dfn=true;
}
bool minus_mat(matrix *a,matrix *b,matrix *c)
{
    if (a->dfn==false||b->dfn==false||c->dfn==true) return false;
    if (a->x!=b->x||a->y!=b->y) return false;

    c->x=a->x;
    c->y=a->y;
    int x=a->x;
    int y=a->y;
    c->content=(int **)malloc(sizeof(int *)*x);
    for (int i=0;i<x;i++)
    {
        c->content[i]=(int *)malloc(sizeof(int)*y);
        for (int j=0;j<y;j++)
            c->content[i][j]=a->content[i][j]-b->content[i][j];
    }
    c->dfn=true;
}
bool gen_mat(matrix *mat,int x,int y)
{
    if (mat->dfn==true) return false;
    mat->x=x;
    mat->y=y;
    srand(time(NULL));
    mat->content=(int **)malloc(sizeof(int *)*x);
    for (int i=0;i<x;i++)
    {
        mat->content[i]=(int *)malloc(sizeof(int)*y);
        for (int j=0;j<y;j++)
        {
            mat->content[i][j]=rand()%1537+512;
        }
    }
    mat->dfn=true;
    return true;
}
bool free_mat(matrix *mat)
{
    if (mat->dfn==false) return false;
    for (int i=0;i<mat->x;i++) free(mat->content[i]);
    free(mat->content);

    mat->dfn=false;
    return;
}


inline int min_pwr(int a)
{
    int ret=1;
    while (a>ret) ret<<=1;
    return ret;
}
matrix *implement_mat(matrix *ori)
{
    int x=min_pwr(ori->x);
    int y=min_pwr(ori->y);

    matrix *ret=(matrix *)malloc(sizeof(matrix));
    ret->x=x;
    ret->y=y;
    ret->content=(int **)malloc(sizeof(int *)*x);
    for (int i=0;i<x;i++)
    {
        ret->content[i]=(int *)malloc(sizeof(int)*y);
        for (int j=0;j<y;j++)//todo: better this loop
        {
            if (i>=ori->x||j>=ori->y) ret->content[i][j]=0;
            else ret->content[i][j]=ori->content[i][j];
        }
    }

    return ret;
}
inline submatrix pack_mat(matrix *ori,part p);
inline submatrix pack_sub(submatrix ori,part p);
bool strassen_add(submatrix a,submatrix b,matrix *c,bool add);
bool strassen_mul(submatrix a,submatrix b,matrix *c);
bool strassen_part_add(matrix *a,matrix *b,submatrix c,bool add);