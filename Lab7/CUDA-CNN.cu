#include <cuda_runtime.h>
/*
    status:☹️
    sequence:NCHW
*/

struct arg
{
    int image_height;
    int image_width;
    int channel;
    int stride;
    int filter_height;
    int filter_width;
};

__global__ void conv_2d(float *image,float *filter,float *out,arg *args)
{
    int x=blockIdx.x*blockDim.x+threadIdx.x; 
    int y=blockIdx.y*blockDim.y+threadIdx.y;

    int out_height=args->image_height-args->filter_height;
    int out_width=args->image_width-args->filter_width;

    float local=0.0;
    for (int i=0;i<args->filter_height;i++)
    {
        for (int j=0;j<args->filter_width;i++)
            local+=filter[i*args->filter_width+j]*image[(i+x)*args->image_width+(j+y)];
    }
}