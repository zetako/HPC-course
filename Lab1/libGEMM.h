#ifndef LIB_GEMM
#define LIB_GEMM

/* a simple bool emulation */
/* why not boolen.h?
** 1. not familiar with it
** 2. may use to pass more out
*/
typedef char bool;  //use it only for return value!
#define true 1
#define false 0

/* matrix struct for better arg pass */
typedef struct matrix
{
    int **content;  //the 2-d pointer
    int x;          //the 1st dimension length
    int y;          //the 2nd dimension length

    bool dfn;       //true=have been defined
} matrix;

/*========== functions to be call ==========*/
//basic matrix multiple
bool basic_multi(matrix *a,matrix *b,matrix *c);

//basic matrix multiple with openMP
bool omp_multi(matrix *a,matrix *b,matrix *c);

//matrix multiple with Strassen algorithm
bool strassen_multi(matrix *a,matrix *b,matrix *c);

/*========== matrix operation ==========*/
//add matrixs, return a **new** matrix
bool add_mat(matrix *a,matrix *b,matrix *c);

//minus matrixs, return a **new** matrix
bool minus_mat(matrix *a,matrix *b,matrix *c);

//return a matrix(x,y) with random value
bool gen_mat(matrix *mat,int x,int y);

//free the target matrix, we check dfn, so basically safe to "double free"
bool free_mat(matrix *mat);


/*========== for stressen algorithm ==========*/
#define STD_MULTI basic_multi   //the default method to fallback
#define strassen_fallback 64    //the size for strassen to fallback

/* matrix struct for better arg pass */
typedef struct submatrix
{
    matrix *parent;     //the matrix itself
    int startX,startY;  //start point
    int lenX,lenY;      //lengths

    bool dfn;           //true=defined, also check parent->dfn plz
}submatrix;

/* enum for pass submatrix position */
typedef enum part
{
    full,
    left_top,       //also call 11
    right_top,      //also call 12
    left_bottom,    //also call 21
    right_bottom,   //also call 22
}part;

//return the min power of 2 that not less than a
inline int min_pwr(int a);

//return a matrix of 2^n size based on ori matrix
matrix *implement_mat(matrix *ori);

//pack matrix to submatrix based on position
inline submatrix pack_mat(matrix *ori,part p);

//pack submatrix again based on position
inline submatrix pack_sub(submatrix ori,part p);

//form new matrix from submatrix
matrix *get_mat_from_sub(submatrix ori);

//add used in strassen algorithm's 1st step
bool strassen_add(submatrix a,submatrix b,matrix *c,bool add);

//multiple used in strassen algorithm's 2nd step, also recursive
bool strassen_mul(submatrix a,submatrix b,matrix *c);

//add used in strassen algorithm's 3rd step
bool strassen_part_add(submatrix a,matrix *b,bool add);


#endif