#ifndef USECUDAAPI_H
#define USECUDAAPI_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 运行CUDA向量加法（修改为unsigned char版本）
 *
 * @param a 输入向量A
 * @param b 输入向量B
 * @param c 输出向量C（结果）
 * @param n 向量大小
 * @return bool 成功返回true，失败返回false
 */
bool runCudaVectorAdd(unsigned char* a, unsigned char* b, unsigned char* c, int n);

/**
 * @brief 获取CUDA设备信息
 *
 * @return char* CUDA设备信息字符串（需要调用者释放内存）
 */
char* getCudaDeviceInfo();

#ifdef __cplusplus
}
#endif

#endif // USECUDAAPI_H
