#ifndef LIB_GEMM
#define LIB_GEMM

/* I WANT BOOL!!! */
#define true 1
#define false 0
typedef char bool;

/* matrix struct for better arg pass */
typedef struct matrix
{
    int **content;
    int x;
    int y;

    bool dfn;
} matrix;

/* functions to be call */
bool basic_multi(matrix *a,matrix *b,matrix *c);
bool omp_multi(matrix *a,matrix *b,matrix *c);
bool strassen_multi(matrix *a,matrix *b,matrix *c);
bool add_mat(matrix *a,matrix *b,matrix *c);
bool minus_mat(matrix *a,matrix *b,matrix *c);
bool gen_mat(matrix *mat,int x,int y);
bool free_mat(matrix *mat);


/* for stressen algorithm */
#define STD_MULTI basic_multi
#define strassen_fallback 64

typedef struct submatrix
{
    matrix *parent;
    int startX,startY;
    int lenX,lenY;

    bool dfn;
}submatrix;

typedef enum part
{
    full,
    left_top,
    right_top,
    left_bottom,
    right_bottom,
}part;

inline int min_pwr(int a);
matrix *implement_mat(matrix *ori);
inline submatrix pack_mat(matrix *ori,part p);
inline submatrix pack_sub(submatrix ori,part p);
bool strassen_add(submatrix a,submatrix b,matrix *c,bool add);
bool strassen_mul(submatrix a,submatrix b,matrix *c);
bool strassen_part_add(matrix *a,matrix *b,submatrix c,bool add);


#endif