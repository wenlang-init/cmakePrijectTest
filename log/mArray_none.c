#include "mArray_none.h"
#include "mem_management.h"

#ifndef NULL
#define NULL 0
#endif // !NULL

// 数组最大节点数
#define INIT_LIST_NODE_NUM 1024*100

/*
    功能：
        初始化数组
    参数：
    返回值：
        成功：数组指针
        失败：NULL
*/
Array_None *init_array_none()
{
    Array_None *array = (Array_None *)mem_malloc(sizeof(Array_None));
    if(array == NULL){
        return NULL;
    }

    array->ro.node_max = INIT_LIST_NODE_NUM;
    array->ro.index = (Array_Node_None *)mem_malloc(sizeof(Array_Node_None)*array->ro.node_max);
    if(array->ro.index == NULL){
        mem_free(array);
        return NULL;
    }

    array->ro.count = 0;

    // 数组方法
    array->func.insert_head = array_none_insert_node_head;
    array->func.append = array_none_insert_node_tail;
    array->func.insert = array_none_insert_at;

    array->func.remove = array_none_remove_node_at;
    array->func.remove_d = array_none_remove_node_at_d;
    array->func.remove_node = array_none_remove_node;
    array->func.remove_node_d = array_none_remove_node_d;

    array->func.clear = array_none_delete_all_node;
    array->func.clear_d = array_none_delete_all_node_d;

    array->func.destroy = array_none_destroy;
    array->func.destroy_d = array_none_destroy_d;

    array->func.get_node_index = array_none_get_node_index;
    array->func.at = array_none_get_node_at;

    array->func.find_node_index = array_none_find_node_index;

    array->func.swap = array_none_swap_node;
    array->func.move = array_none_move_node;

    return array;
}
/*
功能：添加到头部
参数：
    obj:数组对象
    data:插入的数据的指针
返回值：
    成功:对应的节点
    失败:NULL
*/
Array_Node_None *array_none_insert_node_head(Array_None *obj,void *data)
{
    if(obj == NULL){
        return NULL;
    }
    if(obj->ro.count >= obj->ro.node_max){
        return NULL;
    }

    // 全部向后移动一位
    int i;
    for(i=obj->ro.count-1;i>=0;i--){
        obj->ro.index[i+1] = obj->ro.index[i];
    }

    Array_Node_None *node = &obj->ro.index[0];
    node->data = data;

    obj->ro.count++;

    return node;
}
/*
功能：添加节点到尾部
参数：
    obj:数组对象
    data:插入的数据的指针
返回值：
    成功:对应的节点
    失败:NULL
*/
Array_Node_None *array_none_insert_node_tail(Array_None *obj,void *data)
{
    if(obj == NULL){
        return NULL;
    }
    if(obj->ro.count >= obj->ro.node_max){
        return NULL;
    }
    
    Array_Node_None *node = &obj->ro.index[obj->ro.count];
    node->data = data;

    obj->ro.count++;

    return node;
}
/*
功能：插入节点到index
参数：
    obj:数组对象
    index:插入的位置(0到最后范围，超过则插入最近位置)
    data:插入的数据的指针
返回值：
    成功:对应的节点
    失败:NULL
*/
Array_Node_None *array_none_insert_at(Array_None *obj,int index,void *data)
{
    if(obj == NULL){
        return NULL;
    }
    if(obj->ro.count >= obj->ro.node_max){
        return NULL;
    }
    if(index < 0){
        index = 0;
    } else if(index > obj->ro.count){
        index = obj->ro.count;
    }

    // index及后全部向后移动一位
    int i;
    for(i=obj->ro.count-1;i>=index;i--){
        obj->ro.index[i+1] = obj->ro.index[i];
    }

    Array_Node_None *node = &obj->ro.index[index];
    node->data = data;

    obj->ro.count++;

    return node;
}
/*
功能：删除index处的节点
参数：
    obj:数组对象
    index:删除的位置(0到最后范围，超过不处理)
返回值：
    成功:0
    失败:-1成功，-2删除成功扩展内存失败
*/
int array_none_remove_node_at(Array_None *obj,int index){
    if(obj == NULL){
        return -1;
    }

    if(index < 0 || index >= obj->ro.count){
        return -1;
    }

    // index后全部向前移动一位
    int i;
    for(i=index;i<obj->ro.count-1;i++){
        obj->ro.index[i] = obj->ro.index[i+1];
    }

    obj->ro.count--;

    return 0;
}
/*
功能：删除index处的节点(包含data)
参数：
    obj:数组对象
    index:删除的位置(0到最后范围，超过不处理)
返回值：
    成功:0
    失败:-1
*/
int array_none_remove_node_at_d(Array_None *obj,int index){
    if(obj == NULL){
        return -1;
    }

    if(index < 0 || index >= obj->ro.count){
        return -1;
    }

    if(obj->ro.index[index].data != NULL){
        mem_free(obj->ro.index[index].data);
    }
    // index后全部向前移动一位
    int i;
    for(i=index;i<obj->ro.count-1;i++){
        obj->ro.index[i] = obj->ro.index[i+1];
    }
    
    obj->ro.count--;

    return 0;
}
/*
功能：删除node节点
参数：
    obj:数组对象
    node:删除的节点
返回值：
    成功:0
    失败:-1
*/
int array_none_remove_node(Array_None *obj,Array_Node_None *node)
{
    if(obj == NULL || node == NULL){
        return -1;
    }
    int index = array_none_get_node_index(obj,node);
    if(index < 0){
        return -1;
    }
    return array_none_remove_node_at(obj,index);
}
/*
功能：删除node节点(包含data)
参数：
    obj:数组对象
    node:删除的节点
返回值：
    成功:0
    失败:-1
*/
int array_none_remove_node_d(Array_None *obj,Array_Node_None *node)
{
    if(obj == NULL || node == NULL){
        return -1;
    }
    int index = array_none_get_node_index(obj,node);
    if(index < 0){
        return -1;
    }
    return array_none_remove_node_at_d(obj,index);
}
/*
功能：删除链表全部节点
参数：
    obj:数组对象
返回值：无
*/
void array_none_delete_all_node(Array_None *obj)
{
    if(obj == NULL){
        return;
    }

    obj->ro.count = 0;
}
/*
功能：删除链表全部节点(包含data)
参数：
    obj:数组对象
返回值：无
*/
void array_none_delete_all_node_d(Array_None *obj)
{
    if(obj == NULL){
        return;
    }

    int i;
    for(i=0;i<obj->ro.count;i++){
        if(obj->ro.index[i].data != NULL){
            mem_free(obj->ro.index[i].data);
        }
    }

    obj->ro.count = 0;
}
/*
功能：删除链表
参数：
    obj:数组对象
返回值：无
*/
void array_none_destroy(Array_None *obj)
{
    if(obj == NULL){
        return;
    }
    mem_free(obj->ro.index);
    mem_free(obj);
}
/*
功能：删除链表,包含data
参数：
    obj:数组对象
返回值：无
*/
void array_none_destroy_d(Array_None *obj)
{
    if(obj == NULL){
        return;
    }
    int i;
    for(i=0;i<obj->ro.count;i++){
        if(obj->ro.index[i].data != NULL){
            mem_free(obj->ro.index[i].data);
        }
    }
    mem_free(obj->ro.index);
    mem_free(obj);
}
/*
功能：获取节点的index
参数：
    obj:数组对象
    index:获取的位置(0到最后范围，超过不处理)
返回值：
    成功:对应的节点index
    失败:-1
*/
int array_none_get_node_index(Array_None *obj,Array_Node_None *node)
{
    if(obj == NULL || node == NULL){
        return -1;
    }
    int i;
    for(i=0;i<obj->ro.count;i++){
        if(node == &obj->ro.index[i]){
            return i;
        }
    }
    return -1;
} 
/*
功能：获取第index个节点
参数：
    obj:数组对象
    index:获取的位置(0到最后范围，超过不处理)
返回值：
    成功:对应的节点
    失败:NULL
*/
Array_Node_None *array_none_get_node_at(Array_None *obj,int index)
{
    if(obj == NULL){
        return NULL;
    }
    if(index < 0 || index >= obj->ro.count){
        return NULL;
    }
    return &obj->ro.index[index];
}
/*
功能：根据data指针查找node,要求表中数据指针唯一，否则返回第一个找到的,没找到返回负数
参数：
    obj:数组对象
    data:要查找的数据的指针
返回值：
    成功:找到的位置
    失败:-1
*/
int array_none_find_node_index(Array_None *obj,void *data)
{
    if(obj == NULL){
        return -1;
    }
    int i;
    for(i=0;i<obj->ro.count;i++){
        if(data == obj->ro.index[i].data){
            return i;
        }
    }
    return -1;
}
/*
功能：交换i和j
参数：
    obj:数组对象
    i,j:要交换的位置
返回值：
    成功:0
    失败:-1
*/
int array_none_swap_node(Array_None *obj,int i,int j)
{
    if(obj == NULL){
        return -1;
    }
    if(i < 0 || i >= obj->ro.count || j < 0 || j >= obj->ro.count){
        return -1;
    }

    Array_Node_None node = obj->ro.index[i];
    obj->ro.index[i] = obj->ro.index[j];
    obj->ro.index[j] = node;

    return 0;
}
/*
功能：移动i到j
参数：
    obj:数组对象
    i,j:要移动的位置
返回值：
    成功:0
    失败:-1
*/
int array_none_move_node(Array_None *obj,int i,int j)
{
    if(obj == NULL){
        return -1;
    }
    if(i < 0 || i >= obj->ro.count || j < 0 || j >= obj->ro.count){
        return -1;
    }
    if(i < j){
        // 右移,i到j间的数据和j前移一位
        Array_Node_None node = obj->ro.index[i];
        int k;
        for(k=i;k<j;k++){
            obj->ro.index[k] = obj->ro.index[k+1];
        }
        obj->ro.index[j] = node;
    } else if(i > j){
        // 左移,i到j间的数据和j后移一位
        Array_Node_None node = obj->ro.index[i];
        int k;
        for(k=i;k>j;k--){
            obj->ro.index[k] = obj->ro.index[k-1];
        }
        obj->ro.index[j] = node;
    }
    return 0;
}

void test(){
    Array_None *array = init_array_none();
	if(array == NULL){
		return;
	}
	int i;
	int data[10];
	for(i=0;i<10;i++){
		data[i] = i;
		array->func.append(array,&data[i]);
	}

	//array->func.swap(array,1,3);
	array->func.move(array,3,5);
	
	for(i=0;i<array->ro.count;i++){
		//printf("%d\n",*(int*)array->ro.index[i].data);
	}
	array->func.destroy(array);
}

