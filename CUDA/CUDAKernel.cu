#include <cuda_runtime.h>
#include <stdio.h>
#include <stdlib.h>  // 添加这个头文件用于malloc

/**
 * @brief 检查CUDA错误
 */
bool checkCudaError(cudaError_t err, const char* msg)
{
    if (err != cudaSuccess) {
        printf("CUDA Error: %s - %s\n", msg, cudaGetErrorString(err));
        return false;
    }
    return true;
}

/**
 * @brief 清理设备内存
 */
void cleanup(unsigned char* d_a, unsigned char* d_b, unsigned char* d_c)
{
    if (d_a) cudaFree(d_a);
    if (d_b) cudaFree(d_b);
    if (d_c) cudaFree(d_c);
}

/**
 * CUDA核函数：向量加法（使用unsigned char类型）
 *
 * 注意：这里使用unsigned char是因为mainwindow中使用的是quint8
 */
__global__ void vectorAddKernel(unsigned char* a, unsigned char* b, unsigned char* c, int n)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n) {
        // 注意：这里可能会有溢出问题，因为unsigned char范围是0-255
        // 我们使用模运算来防止溢出
        c[i] = (a[i] + b[i]) % 256;
    }
}

/**
 * @brief CUDA向量加法主函数（修改为unsigned char版本）
 */
extern "C" bool cuda_vector_add(unsigned char* a, unsigned char* b, unsigned char* c, int n)
{
    cudaError_t err = cudaSuccess;

    // 1. 分配设备内存
    unsigned char *d_a = nullptr;
    unsigned char *d_b = nullptr;
    unsigned char *d_c = nullptr;

    err = cudaMalloc((void**)&d_a, n * sizeof(unsigned char));
    if (!checkCudaError(err, "分配d_a内存失败")) return false;

    err = cudaMalloc((void**)&d_b, n * sizeof(unsigned char));
    if (!checkCudaError(err, "分配d_b内存失败")) {
        cudaFree(d_a);
        return false;
    }

    err = cudaMalloc((void**)&d_c, n * sizeof(unsigned char));
    if (!checkCudaError(err, "分配d_c内存失败")) {
        cudaFree(d_a);
        cudaFree(d_b);
        return false;
    }

    // 2. 复制数据从主机到设备
    err = cudaMemcpy(d_a, a, n * sizeof(unsigned char), cudaMemcpyHostToDevice);
    if (!checkCudaError(err, "复制a到设备失败")) {
        cleanup(d_a, d_b, d_c);
        return false;
    }

    err = cudaMemcpy(d_b, b, n * sizeof(unsigned char), cudaMemcpyHostToDevice);
    if (!checkCudaError(err, "复制b到设备失败")) {
        cleanup(d_a, d_b, d_c);
        return false;
    }

    // 3. 配置线程块和网格大小
    int threadsPerBlock = 256;
    int blocksPerGrid = (n + threadsPerBlock - 1) / threadsPerBlock;

    printf("CUDA配置: %d个块 x %d个线程/块\n", blocksPerGrid, threadsPerBlock);

    // 4. 启动CUDA核函数
    vectorAddKernel<<<blocksPerGrid, threadsPerBlock>>>(d_a, d_b, d_c, n);

    // 检查核函数启动错误
    err = cudaGetLastError();
    if (!checkCudaError(err, "启动核函数失败")) {
        cleanup(d_a, d_b, d_c);
        return false;
    }

    // 5. 等待所有CUDA操作完成
    err = cudaDeviceSynchronize();
    if (!checkCudaError(err, "等待设备同步失败")) {
        cleanup(d_a, d_b, d_c);
        return false;
    }

    // 6. 复制结果从设备到主机
    err = cudaMemcpy(c, d_c, n * sizeof(unsigned char), cudaMemcpyDeviceToHost);
    if (!checkCudaError(err, "复制结果回主机失败")) {
        cleanup(d_a, d_b, d_c);
        return false;
    }

    // 7. 释放设备内存
    cleanup(d_a, d_b, d_c);

    return true;
}

/**
 * @brief 获取CUDA设备信息
 */
extern "C" char* cuda_get_device_info()
{
    cudaError_t err = cudaSuccess;
    cudaDeviceProp prop;
    int deviceCount = 0;

    err = cudaGetDeviceCount(&deviceCount);
    if (err != cudaSuccess || deviceCount == 0) {
        char* msg = (char*)malloc(100);
        snprintf(msg, 100, "没有找到可用的CUDA设备");
        return msg;
    }

    err = cudaGetDeviceProperties(&prop, 0);
    if (err != cudaSuccess) {
        char* msg = (char*)malloc(100);
        snprintf(msg, 100, "获取设备属性失败");
        return msg;
    }

    // 分配足够的内存来存储设备信息
    char* info = (char*)malloc(512);
    snprintf(info, 512,
        "CUDA设备信息:\n"
        "  设备名称: %s\n"
        "  CUDA核心数: %d\n"
        "  计算能力: %d.%d\n"
        "  全局内存: %.2f GB\n"
        "  共享内存/块: %zu KB\n"
        "  最大线程数/块: %d\n"
        "  块最大尺寸: (%d, %d, %d)\n"
        "  网格最大尺寸: (%d, %d, %d)",
        prop.name,
        prop.multiProcessorCount * 128, // 近似核心数
        prop.major, prop.minor,
        prop.totalGlobalMem / (1024.0 * 1024.0 * 1024.0),
        prop.sharedMemPerBlock / 1024,
        prop.maxThreadsPerBlock,
        prop.maxThreadsDim[0], prop.maxThreadsDim[1], prop.maxThreadsDim[2],
        prop.maxGridSize[0], prop.maxGridSize[1], prop.maxGridSize[2]
    );
    return info;
}
