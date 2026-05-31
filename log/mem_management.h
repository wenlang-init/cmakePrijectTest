#ifndef _MEM_MANAGEMENT_H
#define _MEM_MANAGEMENT_H

#ifdef __cplusplus
extern "C" {
#endif

// 内存块大小
#define MEM_BLOCK_SIZE 32
// 内存池大小
#define MEM_MAX_SIZE 1024*1024
// 内存表大小
#define MEM_TABLE_SIZE MEM_MAX_SIZE/MEM_BLOCK_SIZE

// 申请内存
void *mem_malloc(unsigned int size);
// 释放内存
void mem_free(void *ptr);
// 获取内存使用率（mem_malloc可使用内存的使用率）
double mem_peruser(void);
// 重置mem_malloc，之前通过mem_malloc申请的内存将不再可用
void rest_malloc(void);

#ifdef __cplusplus
}
#endif

#endif
