#ifndef _FOPS_H
#define _FOPS_H

/// 当打开文件时，执行scull_open
extern int scull_open(struct inode *inode, struct file *filp);
/// 当关闭文件时，执行scull_release
extern int scull_release(struct inode *inode, struct file *filp);
/// 当读取文件时，调用read时，执行scull_read方法
extern ssize_t scull_read(struct file *filp, char __user *buff, size_t count, loff_t *offp);
/// 当write文件时，执行
extern ssize_t scull_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp);
/// 释放scull设备占用的内存区域
extern void scull_trim(struct scull_dev *dev);

#endif
