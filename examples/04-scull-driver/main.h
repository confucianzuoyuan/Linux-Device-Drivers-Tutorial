#ifndef _MAIN_H
#define _MAIN_H

/// 双向链表
#include <linux/list.h> /* double linked list support */

/// 驱动名称
#define MODULE_NAME "scull"
/// 设备的数量
#define SCULL_NR_DEVS 3
/// 使用链表维护block，每个block的大小是一页的大小
#define SCULL_BLOCK_SIZE PAGE_SIZE // one page per block

/// block结构体
struct scull_block
{
    loff_t offset;
    char data[SCULL_BLOCK_SIZE];
    /// linux中将链表嵌入到数据里面
    struct list_head block_list;
};

/// scull设备
struct scull_dev
{
    int block_counter; // record how many blocks now in the list
    /// 互斥锁
    struct mutex mutex;
    /// 字符设备结构体
    struct cdev cdev;
    /// 将链表嵌入到数据中
    struct list_head block_list; // list of storage blocks
};

#endif
