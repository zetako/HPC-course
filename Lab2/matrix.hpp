#ifndef MATRIX_HPP
#define MATRIX_HPP 1

struct matrix
{
    int x,y;        //2-dimension
    int *content;   //size is x*y
    bool generated; //gen or not

    bool gen();     //randomly generate

    matrix(int x,int y);
    ~matrix();
};

#endif