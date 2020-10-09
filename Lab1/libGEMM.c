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
    ret->dfn=true;

    return ret;
}
inline submatrix pack_mat(matrix *ori,part p)
{
    submatrix ret;
    ret.parent=ori;
    switch (p)
    {
    case left_top:
        ret.startX=ret.startY=0;
        ret.lenX=ori->x/2;
        ret.lenY=ori->y/2;
        break;
    case right_top:
        ret.startX=ori->x/2;
        ret.startY=0;
        ret.lenX=ori->x/2;
        ret.lenY=ori->y/2;
        break;
    case left_bottom:
        ret.startX=0;
        ret.startY=ori->y/2;
        ret.lenX=ori->x/2;
        ret.lenY=ori->y/2;
        break;
    case right_bottom:
        ret.startX=ori->x/2;
        ret.startY=ori->y/2;
        ret.lenX=ori->x/2;
        ret.lenY=ori->y/2;
        break;
    case full:
    default:
        ret.startX=ret.startY=0;
        ret.lenX=ori->x;
        ret.lenY=ori->y;
        break;
    }
    ret.dfn=true;
    return ret;
}
inline submatrix pack_sub(submatrix ori,part p)
{
    submatrix ret;
    ret.parent=ori.parent;
    switch (p)
    {
    case left_top:
        ret.lenX=ori.lenX/2;
        ret.lenY=ori.lenY/2;
        ret.startX=ori.startX;
        ret.startY=ori.startY;
        break;
    case right_top:
        ret.lenX=ori.lenX/2;
        ret.lenY=ori.lenY/2;
        ret.startX=ori.startX+ret.lenX;
        ret.startY=ori.startY;
        break;
    case left_bottom:
        ret.lenX=ori.lenX/2;
        ret.lenY=ori.lenY/2;
        ret.startX=ori.startX;
        ret.startY=ori.startY+ret.lenY;
        break;
    case right_bottom:
        ret.lenX=ori.lenX/2;
        ret.lenY=ori.lenY/2;
        ret.startX=ori.startX+ret.lenX;
        ret.startY=ori.startY+ret.lenY;
        break;
    case full:  //normally, we don't expect passing "full" to this func 
    default:
        ret.lenX=ori.lenX;
        ret.lenY=ori.lenY;
        ret.startX=ori.startX;
        ret.startY=ori.startY;
        break;
    }
    ret.dfn=true;
    return ret;
}
matrix *get_mat_from_sub(submatrix ori);
bool strassen_add(submatrix a,submatrix b,matrix *c,bool add);
bool strassen_mul(submatrix a,submatrix b,matrix *c)
{
    /* verify matrixs */
    if (c->dfn==true) return false;
    if (a.dfn==false||b.dfn==false) return false;
    /* verify a,b's size */
    if (a.lenY!=b.lenX) return false;
    /* check size to determine fallback */
    int x=a.lenX;
    int y=b.lenY;
    if (x<=strassen_fallback||y<=strassen_fallback)
    {
        matrix *new_a=get_mat_from_sub(a);
        matrix *new_b=get_mat_from_sub(b);

        bool status=STD_MULTI(new_a,new_b,c);
        
        free_mat(new_a);
        free_mat(new_b);

        return status;
    }

    /* step 1: do add/minus */
    matrix s1;  //s1=b12-b22
    s1.dfn=false;
    bool status=strassen_add(pack_sub(b,right_top),pack_sub(b,right_bottom),&s1,false);
    if (status!=true) return false;

    matrix s2;  //s2=a11+a12
    s2.dfn=false;
    status=strassen_add(pack_sub(a,left_top),pack_sub(a,right_top),&s2,true);
    if (status!=true) return false;

    matrix s3;  //s3=a21+a22
    s3.dfn=false;
    status=strassen_add(pack_sub(a,left_bottom),pack_sub(a,right_bottom),&s3,true);
    if (status!=true) return false;

    matrix s4;  //s4=b21-b11
    s4.dfn=false;
    status=strassen_add(pack_sub(b,left_bottom),pack_sub(b,left_top),&s4,false);
    if (status!=true) return false;

    matrix s5;  //s5=a11+a22
    s5.dfn=false;
    status=strassen_add(pack_sub(a,left_top),pack_sub(a,right_bottom),&s5,true);
    if (status!=true) return false;

    matrix s6;  //s6=b11+b22
    s6.dfn=false;
    status=strassen_add(pack_sub(b,left_top),pack_sub(b,right_bottom),&s6,true);
    if (status!=true) return false;

    matrix s7;  //s7=a12-a22
    s7.dfn=false;
    status=strassen_add(pack_sub(a,right_top),pack_sub(a,right_bottom),&s7,false);
    if (status!=true) return false;

    matrix s8;  //s8=b21+b22
    s8.dfn=false;
    status=strassen_add(pack_sub(b,left_bottom),pack_sub(b,right_bottom),&s8,true);
    if (status!=true) return false;

    matrix s9;  //s9=a11-a21
    s9.dfn=false;
    status=strassen_add(pack_sub(a,left_top),pack_sub(a,left_bottom),&s9,false);
    if (status!=true) return false;

    matrix s10;  //s10=b11+b12
    s10.dfn=false;
    status=strassen_add(pack_sub(b,left_top),pack_sub(b,right_top),&s10,true);
    if (status!=true) return false;

    /* step 2: do multiple/recursive */
    matrix p1;
    p1.dfn=false;
    status=strassen_mul(pack_sub(a,left_top),pack_mat(&s1,full),&p1);
    if (status!=true) return false;

    matrix p2;
    p2.dfn=false;
    status=strassen_mul(pack_mat(&s2,full),pack_sub(b,right_bottom),&p2);
    if (status!=true) return false;

    matrix p3;
    p3.dfn=false;
    status=strassen_mul(pack_mat(&s2,full),pack_sub(b,left_top),&p3);
    if (status!=true) return false;

    matrix p4;
    p4.dfn=false;
    status=strassen_mul(pack_sub(a,right_bottom),pack_mat(&s4,full),&p4);
    if (status!=true) return false;

    matrix p5;
    p5.dfn=false;
    status=strassen_mul(pack_mat(&s5,full),pack_mat(&s6,full),&p5);
    if (status!=true) return false;

    matrix p6;
    p6.dfn=false;
    status=strassen_mul(pack_mat(&s7,full),pack_mat(&s7,full),&p6);
    if (status!=true) return false;

    matrix p7;
    p7.dfn=false;
    status=strassen_mul(pack_mat(&s9,full),pack_mat(&s10,full),&p7);
    if (status!=true) return false;

    /* step 3: final add/minus */
    c->content=(int **)malloc(sizeof(int *)*x);
    for (int i=0;i<x;i++)
    {
        c->content[i]=(int *)malloc(sizeof(int)*y);
        for (int j=0;j<y;j++) c->content[i][j]=0;
    }
    c->dfn=true;

    submatrix c11=pack_mat(c,left_top);
    status=strassen_part_add(c11,&p5,true);
    if (status!=true) return false;
    status=strassen_part_add(c11,&p4,true);
    if (status!=true) return false;
    status=strassen_part_add(c11,&p2,false);
    if (status!=true) return false;
    status=strassen_part_add(c11,&p6,true);
    if (status!=true) return false;

    submatrix c12=pack_mat(c,right_top);
    status=strassen_part_add(c12,&p1,true);
    if (status!=true) return false;
    status=strassen_part_add(c12,&p2,true);
    if (status!=true) return false;

    submatrix c21=pack_mat(c,left_bottom);
    status=strassen_part_add(c21,&p3,true);
    if (status!=true) return false;
    status=strassen_part_add(c21,&p4,true);
    if (status!=true) return false;

    submatrix c22=pack_mat(c,right_bottom);
    status=strassen_part_add(c22,&p5,true);
    if (status!=true) return false;
    status=strassen_part_add(c22,&p1,true);
    if (status!=true) return false;
    status=strassen_part_add(c22,&p3,false);
    if (status!=true) return false;
    status=strassen_part_add(c22,&p7,false);

    /* step 4: free spaces! */
    free_mat(&s1);
    free_mat(&s2);
    free_mat(&s3);
    free_mat(&s4);
    free_mat(&s5);
    free_mat(&s6);
    free_mat(&s7);
    free_mat(&s8);
    free_mat(&s9);
    free_mat(&s10);
    free_mat(&p1);
    free_mat(&p2);
    free_mat(&p3);
    free_mat(&p4);
    free_mat(&p5);
    free_mat(&p6);
    free_mat(&p7);
}
bool strassen_part_add(submatrix a,matrix *b,bool add);