#include "UseCudaApi.h"

extern "C" {
    bool cuda_vector_add(unsigned char* a, unsigned char* b, unsigned char* c, int n);
    char* cuda_get_device_info();
}

bool runCudaVectorAdd(unsigned char* a, unsigned char* b, unsigned char* c, int n)
{
    // 调用CUDA函数
    return cuda_vector_add(a, b, c, n);
}

char* getCudaDeviceInfo()
{
    return cuda_get_device_info();
}
