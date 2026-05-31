#ifndef _MARRAY_NONE_H
#define _MARRAY_NONE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _arrayobj_None Array_None;
typedef struct _arraynode_None Array_Node_None;

struct _arraynode_None{
    void* data;
};

struct _arrayobj_None{
    struct _arrayobjro_none
    {
        int count;
        int node_max; // index的最大指针数（链表最大节点数）
        Array_Node_None *index; // 数组存储空间
    }ro;

    struct _arrayobjfunction
    {
        Array_Node_None *(*insert_head)(Array_None *obj,void *data); // 添加到头部
        Array_Node_None *(*append)(Array_None *obj,void *data); // 添加到尾部
        Array_Node_None *(*insert)(Array_None *obj,int index,void *data); // 插入节点到index

        int (*remove)(Array_None *obj,int index); // 删除index处的节点
        int (*remove_d)(Array_None *obj,int index); // 删除index处的节点(包含data)
        int (*remove_node)(Array_None *obj,Array_Node_None *node); // 删除node节点
        int (*remove_node_d)(Array_None *obj,Array_Node_None *node); // 删除node节点(包含data)

        void (*clear)(Array_None *obj); // 删除链表全部节点
        void (*clear_d)(Array_None *obj); // 删除链表全部节点(包含data)

        void (*destroy)(Array_None *obj); // 删除链表
        void (*destroy_d)(Array_None *obj); // 删除链表,包含data

        int (*get_node_index)(Array_None *obj,Array_Node_None *node); // 获取节点的index
        Array_Node_None *(*at)(Array_None *obj,int index); // 获取第index个节点

        int (*find_node_index)(Array_None *obj,void *data); // 根据data指针查找node,要求表中数据指针唯一，否则返回第一个找到的,没找到返回负数
        
        int (*swap)(Array_None *obj,int i,int j); // 交换i和j
        int (*move)(Array_None *obj,int i,int j); // 移动i到j
    }func;
    
};

Array_None *init_array_none(); // 初始化链表

Array_Node_None *array_none_insert_node_head(Array_None *obj,void *data); // 添加到头部
Array_Node_None *array_none_insert_node_tail(Array_None *obj,void *data); // 添加节点到尾部
Array_Node_None *array_none_insert_at(Array_None *obj,int index,void *data); // 插入节点到index

int array_none_remove_node_at(Array_None *obj,int index); // 删除index处的节点
int array_none_remove_node_at_d(Array_None *obj,int index); // 删除index处的节点(包含data)

int array_none_remove_node(Array_None *obj,Array_Node_None *node); // 删除node节点
int array_none_remove_node_d(Array_None *obj,Array_Node_None *node); // 删除node节点(包含data)

void array_none_delete_all_node(Array_None *obj); // 删除链表全部节点
void array_none_delete_all_node_d(Array_None *obj); // 删除链表全部节点(包含data)

void array_none_destroy(Array_None *obj); // 删除链表
void array_none_destroy_d(Array_None *obj); // 删除链表,包含data

int array_none_get_node_index(Array_None *obj,Array_Node_None *node); // 获取节点的index
Array_Node_None *array_none_get_node_at(Array_None *obj,int index); // 获取第index个节点

int array_none_find_node_index(Array_None *obj,void *data); // 根据data指针查找node,要求表中数据指针唯一，否则返回第一个找到的,没找到返回负数

int array_none_swap_node(Array_None *obj,int i,int j); // 交换i和j
int array_none_move_node(Array_None *obj,int i,int j); // 移动i到j

#ifdef __cplusplus
}
#endif

#endif
