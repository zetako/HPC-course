#include <stdio.h>
#include <time.h>
#include "libGEMM.h"

int main()
{
    /* generate matrix */
    matrix a,b,c;
    a.dfn=false;
    b.dfn=false;
    c.dfn=false;

    printf("input M,N,K:");
    int m,n,k;
    scanf("%d %d %d",&m,&n,&k);
    
    bool status=gen_mat(&a,m,n);
    if (status==false) return 1;
    printf("Matrix A(%d*%d) generated\n",m,n);
    status=gen_mat(&b,n,k);
    if (status==false) return 1;
    printf("Matrix B(%d*%d) generated\n",n,k);

    /* calculation & timing */
    clock_t start_clock,end_clock;
    start_clock=clock();
    status=basic_multi(&a,&b,&c);
    if (status==false) return 2;
    end_clock=clock();
    printf("basic matrix multiple time:%lf sec.\n",(double)(end_clock-start_clock)/CLOCKS_PER_SEC);

    /* end of prog */
    free_mat(&c);
    free_mat(&b);
    free_mat(&a);

    return 0;
}