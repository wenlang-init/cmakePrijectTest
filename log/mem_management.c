#include "mem_management.h"

#ifndef NULL
#define NULL 0
#endif // !NULL

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

// 内存池
static char mem_buf[MEM_MAX_SIZE];
// 内存表
static unsigned short mem_management_table[MEM_TABLE_SIZE];

struct _mem_man_dev{
    void (*init)(void); // 初始化
    double (*peruser)(void); // 内存使用率
    u8 *membase; // 内存池
    u16 *memmap; // 内存表
    u32 mem_size; // 内存池大小
    u32 memtable_size; // 内存表大小
    u32 memblock_size; // 内存块大小
    unsigned char memrdy; // 内存就绪状态
};

static void mem_init(void); // 初始化
double mem_peruser(void); // 内存使用率

struct _mem_man_dev mem_dev = {
    .init = mem_init,
    .peruser = mem_peruser,
    .membase = (u8*)mem_buf,
    .memmap = mem_management_table,
    .mem_size = MEM_MAX_SIZE,
    .memtable_size = MEM_TABLE_SIZE,
    .memblock_size = MEM_BLOCK_SIZE,
    .memrdy = 0,
};

static void mem_init(void){
    int i;
    for(i=0;i<MEM_MAX_SIZE;i++){
        mem_dev.membase[i] = 0;
    }
    for(i=0;i<MEM_TABLE_SIZE;i++){
        mem_dev.memmap[i] = 0;
    }
    mem_dev.memrdy = 1;
}

static u32 mem_offset(unsigned int size){
    long offset = 0;
    u32 memblock; // 需要的内存块数
    u32 cmemblock=0; // 连续的内存块数
    u32 i;
    if(mem_dev.memrdy == 0){
        mem_dev.init();
    }
    if(size == 0){
        return 0xffffffff;
    }
    memblock = size / mem_dev.memblock_size;
    if(size % mem_dev.memblock_size){
        memblock++;
    }
    offset=mem_dev.memtable_size-1;
    for(offset=mem_dev.memtable_size-1;offset>=0;offset--){
        if(0 == mem_dev.memmap[offset]){
            cmemblock++;
        } else {
            cmemblock = 0;
        }
        if(cmemblock == memblock){
            for(i=0;i<cmemblock;i++){
                mem_dev.memmap[offset+i] = cmemblock;
            }
            return offset*mem_dev.memblock_size;
        }
    }
    return 0xffffffff;
}

void *mem_malloc(unsigned int size){
    u32 offset = mem_offset(size);
    if(offset == 0xffffffff){
        return NULL;
    } else {
        return (void *)(mem_dev.membase + offset);
    }
}

u8 mem_free_c(u32 offset){
    int i,memblock,memblock_count;
    if(mem_dev.memrdy == 0){
        mem_dev.init();
        return 1;
    }
    if(offset < mem_dev.mem_size){
        memblock = offset/mem_dev.memblock_size;
        memblock_count = mem_dev.memmap[memblock];
        for(i=0;i<memblock_count;i++){
            mem_dev.memmap[memblock+i] = 0;
        }
        return 0;
    } else {
        return 2;
    }
}

void mem_free(void *ptr){
    u32 offset;
    if(ptr == NULL || ((u8*)ptr < mem_dev.membase))return;
    offset = (u8*)ptr - mem_dev.membase;
    if(offset < mem_dev.mem_size){
        mem_free_c(offset);
    }
}
#include <stdio.h>
double mem_peruser(void){
    u32 use = 0;
    u32 i;
    if(mem_dev.memrdy == 0){
        mem_dev.init();
        return 0;
    }
    for(i=0;i<mem_dev.memtable_size;i++){
        if(mem_dev.memmap[i] != 0){
            use++;
            //use += mem_dev.memmap[i];
            //i = i + mem_dev.memmap[i]-1;
        }
    }
    return use*100.0/mem_dev.memtable_size;
}

void rest_malloc(void){
    mem_dev.init();
}

