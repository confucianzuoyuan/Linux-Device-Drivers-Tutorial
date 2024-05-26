#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <asm/atomic.h>
#include <asm/uaccess.h>

#include "main.h"
#include "fops.h"

/// 文件open时的操作
int scull_open(struct inode *inode, struct file *filp)
{
    /// 初始化空结构体
    struct scull_dev *dev;

    pr_debug("%s() is invoked\n", __FUNCTION__);

    /// 使用container_of宏，查找包含指针inode->i_cdev的结构体。
    dev = container_of(inode->i_cdev, struct scull_dev, cdev);
    /// 将dev保存到private_data中
    filp->private_data = dev;

    if ((filp->f_flags & O_ACCMODE) == O_WRONLY)
    {
        /// 锁的生命周期要注意
        /// 获取互斥锁
        if (mutex_lock_interruptible(&dev->mutex))
            return -ERESTARTSYS;
        /// 释放内存空间
        scull_trim(dev);
        /// 释放锁
        mutex_unlock(&dev->mutex);
    }

    return 0;
}

/// 文件close时的操作
int scull_release(struct inode *inode, struct file *filp)
{
    pr_debug("%s() is invoked\n", __FUNCTION__);
    return 0;
}

/// 读取文件的操作
ssize_t scull_read(struct file *filp, char __user *buff, size_t count,
                   loff_t *f_pos)
{
    struct scull_dev *dev = filp->private_data;
    // 初始化一个block
    struct scull_block *pblock = NULL;
    // 返回值的默认值是-ENOMEM(内存不足)
    loff_t retval = -ENOMEM;
    loff_t tblock = 0, toffset = 0;
    // 初始化链表
    struct list_head *plist = NULL;

    pr_debug("%s() is invoked\n", __FUNCTION__);

    /// 计算当前block的数量
    tblock = *f_pos / SCULL_BLOCK_SIZE;
    /// 计算偏移量
    toffset = *f_pos % SCULL_BLOCK_SIZE;

    /// 获取锁
    if (mutex_lock_interruptible(&dev->mutex))
        return -ERESTARTSYS;

    /// 错误情况1
    if (tblock + 1 > dev->block_counter)
    {
        retval = 0;
        goto end_of_file;
    }

    /// 继续遍历链表
    plist = &dev->block_list;
    for (int i = 0; i < tblock + 1; ++i)
    {
        plist = plist->next;
    }

    /// 使用list_entry获取链表节点的数据
    pblock = list_entry(plist, struct scull_block, block_list);
    /// 错误情况2
    if (toffset >= pblock->offset)
    {
        retval = 0;
        goto end_of_file;
    }

    if (count > pblock->offset)
        count = pblock->offset;

    // 将block的数据拷贝到用户空间
    if (copy_to_user(buff, pblock->data, count))
    {
        retval = -EFAULT;
        goto cpy_user_error;
    }

    retval = count;
    *f_pos += count;

end_of_file:
cpy_user_error:
    pr_debug("RD pos = %lld, block = %lld, offset = %lld, read %lu bytes\n",
             *f_pos, tblock, toffset, count);

    /// 释放锁
    mutex_unlock(&dev->mutex);
    return retval;
}

ssize_t scull_write(struct file *filp, const char __user *buff, size_t count,
                    loff_t *f_pos)
{
    struct scull_dev *dev = filp->private_data;
    struct scull_block *pblock = NULL;
    loff_t retval = -ENOMEM;
    loff_t tblock = 0, toffset = 0;

    pr_debug("%s() is invoked\n", __FUNCTION__);

    tblock = *f_pos / SCULL_BLOCK_SIZE;
    toffset = *f_pos % SCULL_BLOCK_SIZE;

    /// 获取锁
    if (mutex_lock_interruptible(&dev->mutex))
        return -ERESTARTSYS;

    /*
     * For simplicity, we write one block each write request.
     */
    while (tblock + 1 > dev->block_counter)
    {
        /// 在内核空间为scull_block分配内存
        if (!(pblock = kmalloc(sizeof(struct scull_block), GFP_KERNEL)))
            goto malloc_error;
        /// 将分配的内存置为0
        memset(pblock, 0, sizeof(struct scull_block));
        /// 将block添加到链表尾部
        INIT_LIST_HEAD(&pblock->block_list);
        list_add_tail(&pblock->block_list, &dev->block_list);
        dev->block_counter++;
    }
    pblock = list_last_entry(&dev->block_list, struct scull_block, block_list);

    if (count > SCULL_BLOCK_SIZE - toffset)
        count = SCULL_BLOCK_SIZE - toffset;

    /// 将用户态的数据写入到block中
    if (copy_from_user(pblock->data + toffset, buff, count))
    {
        retval = -EFAULT;
        goto cpy_user_error;
    }

    retval = count;
    pblock->offset += count;
    *f_pos += count;

malloc_error:
cpy_user_error:
    pr_debug("WR pos = %lld, block = %lld, offset = %lld, write %lu bytes\n",
             *f_pos, tblock, toffset, count);

    /// 释放锁
    mutex_unlock(&dev->mutex);
    return retval;
}

/// 释放所有block的内存空间
void scull_trim(struct scull_dev *dev)
{
    struct scull_block *cur = NULL, *tmp = NULL;

    pr_debug("%s() is invoked\n", __FUNCTION__);

    /// `list_for_each_entry_safe`安全的遍历链表
    list_for_each_entry_safe(cur, tmp, &dev->block_list, block_list)
    {
        /// 删除链表节点
        list_del(&cur->block_list);
        memset(cur, 0, sizeof(*cur));
        /// 释放内存
        kfree(cur);
    }
    dev->block_counter = 0;
}
