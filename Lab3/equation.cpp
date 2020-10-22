#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <cmath>

using namespace std;

pthread_cond_t con;
pthread_mutex_t con_lock;

void *threadWork1(void *arg)//count b^2
{
    double *ret;
    ret=new double;
    double *b=(double *)arg;
    *ret=(*b)*(*b);
    pthread_mutex_lock(&con_lock);
    pthread_cond_signal(&con);
    pthread_mutex_unlock(&con_lock);
    pthread_exit((void *)ret);
    return (void *)ret;
}
void *threadWork2(void *arg)//count 4ac
{

    double *ret;
    ret=new double;
    pair<double,double> *p=(pair<double,double> *)arg;
    *ret=4*(p->first)*(p->second);
    pthread_mutex_lock(&con_lock);
    pthread_cond_signal(&con);
    pthread_mutex_unlock(&con_lock);
    pthread_exit((void *)ret);
    return (void *)ret;
}

int main()
{
    //init
    double a,b,c;
    cout<<"Input a,b,c (ax^2+bx+c): ";
    cin>>a>>b>>c;
    cout<<a<<"x^2+"<<b<<"x+"<<c<<endl;

    //prepare threads
    int sigCount=2;
    pthread_mutex_init(&con_lock,0);
    pthread_mutex_lock(&con_lock);
    pthread_cond_init(&con,NULL);

    pthread_t pth1,pth2;
    pthread_create(&pth1,0,threadWork1,(void *)&b);
    pair<double,double> arg(a,c);
    pthread_create(&pth2,0,threadWork2,(void *)&arg);

    //sync
    while(sigCount>0)
    {
        pthread_cond_wait(&con,&con_lock);
        sigCount--;
    }
    pthread_mutex_unlock(&con_lock);
    pthread_cond_destroy(&con);
    pthread_mutex_destroy(&con_lock);

    //finish
    double *_b2,*_4ac;
    pthread_join(pth1,(void **)&_b2);
    pthread_join(pth2,(void **)&_4ac);
    double toSqrt=*_b2-*_4ac;
    delete _b2,_4ac;
    toSqrt=sqrt(toSqrt);
    double x1=(-b+toSqrt)/(2*a);
    double x2=(-b-toSqrt)/(2*a);

    cout<<fixed<<"x1= "<<x1<<", x2= "<<x2<<endl;
    return 0;
}