#include "matrix.hpp"
#include <random>
#include <iostream>
using namespace std;

matrix::matrix(int _x,int _y)
{
    x=_x;
    y=_y;
    content=new int[x*y];
}
matrix::~matrix()
{
    delete[] content;
}

bool matrix::gen()
{
    static random_device rd;
    static default_random_engine e(rd());
    static uniform_int_distribution<int> u(512,2048);

    for (int i=0;i<x;i++)
        for (int j=0;j<y;j++) content[i*y+j]=u(e);

    generated=true;
    return true;
}

void matrix::print()
{
    for (int i=0;i<x;i++)
    {
        for (int j=0;j<y;j++) cout<<content[i*y+j]<<' ';
        cout<<endl;
    }
}