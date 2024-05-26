#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/list.h>

#include "main.h"
#include "fops.h"

/// 默认的主设备编号和次设备编号都为0
static int scull_major = 0, scull_minor = 0;

/// 结构体数组，数组大小是设备的数量
static struct scull_dev *scull_dev[SCULL_NR_DEVS];

/// 文件操作的注册
static struct file_operations scull_fops = {
    .owner = THIS_MODULE, // 文件的owner
    .open = scull_open,
    .read = scull_read,
    .write = scull_write,
    .release = scull_release,
};

/// 初始化scull设备
static void __init init_scull_dev(struct scull_dev *dev)
{
    // block的数量设置为0
    dev->block_counter = 0;

    // 初始化链表
    INIT_LIST_HEAD(&dev->block_list);
    // 初始化一个互斥锁
    mutex_init(&dev->mutex);

    // 初始化cdev结构体
    cdev_init(&dev->cdev, &scull_fops);
    // 设置字符设备的owner
    dev->cdev.owner = THIS_MODULE;
}

// 驱动的入口点函数
static int __init m_init(void)
{
    int err = 0;
    // dev_t是主设备号和次设备号组成的u32
    dev_t devno;

    printk(KERN_WARNING MODULE_NAME " is loaded\n");

    // Alloc device number
    // 动态分配主设备编号
    err = alloc_chrdev_region(&devno, scull_minor, SCULL_NR_DEVS, MODULE_NAME);
    if (err < 0)
    {
        pr_debug("Cant't get major");
        return err;
    }
    // 提取主设备编号
    scull_major = MAJOR(devno);

    // 遍历每一个设备
    for (int i = 0; i < SCULL_NR_DEVS; ++i)
    {
        // 为每个设备分配内核空间中的内存空间
        scull_dev[i] = kmalloc(sizeof(struct scull_dev), GFP_KERNEL);
        if (!scull_dev[i])
        {
            pr_debug("Error(%d): kmalloc failed on scull%d\n", err, i);
            continue;
        }

        // 初始化设备
        init_scull_dev(scull_dev[i]);

        /*
         * The cdev_add() function will make this char device usable
         * in userspace. If you havn't ready to populate this device
         * to its users, DO NOT call cdev_add()
         */
        // 创建设备编号
        devno = MKDEV(scull_major, scull_minor + i);
        // 将设备添加为字符设备
        err = cdev_add(&scull_dev[i]->cdev, devno, 1);
        if (err)
        {
            pr_debug("Error(%d): Adding %s%d error\n", err, MODULE_NAME, i);
            kfree(scull_dev[i]);
            scull_dev[i] = NULL;
        }
    }

    // TODO: unregister chrdev_region here if fail
    return 0;
}

// 驱动的出口点函数
static void __exit m_exit(void)
{
    dev_t devno;
    printk(KERN_WARNING MODULE_NAME " unloaded\n");

    for (int i = 0; i < SCULL_NR_DEVS; ++i)
    {
        /// 删除字符设备
        cdev_del(&scull_dev[i]->cdev);
        /// 释放scull外设内存
        scull_trim(scull_dev[i]);
        /// 释放设备内存
        kfree(scull_dev[i]);
    }

    // 构建设备编号
    devno = MKDEV(scull_major, scull_minor);
    // 删除设备
    unregister_chrdev_region(devno, SCULL_NR_DEVS);
}

// 注册入口点函数
module_init(m_init);
// 注册出口点函数
module_exit(m_exit);

// 开源协议
MODULE_LICENSE("GPL");
MODULE_AUTHOR("d0u9");
MODULE_DESCRIPTION("A simple memory based storage device aims to demonstrate "
                   "basic concepts of char device");
