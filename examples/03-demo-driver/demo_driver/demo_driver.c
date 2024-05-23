#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>

#define DRIVER_MAJOR 188
#define DEVICE_NAME "demo_driver"

static int demo_open(struct inode *inode, struct file *file)
{
  printk(KERN_EMERG "demo open\r\n");
  return 0;
}

static ssize_t demo_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
  printk(KERN_EMERG "demo write\r\n");
  return 0;
}


static ssize_t demo_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
  printk(KERN_EMERG "demo read\r\n");
  return 0;
}


static int demo_close(struct inode *inode, struct file *file)
{
  printk(KERN_EMERG "demo open\r\n");
  return 0;
}

static struct file_operations demo_flops =
{
  .owner = THIS_MODULE,
  .open = demo_open,
  .release = demo_close,
  .read = demo_read,
  .write = demo_write
};

static int __init demo_init(void)
{
  int ret;

  ret = register_chrdev(DRIVER_MAJOR, DEVICE_NAME, &demo_flops);

  if (ret < 0)
  {
    printk(KERN_EMERG DEVICE_NAME " cannot register major number.\r\n");
    return ret;
  }
  else
  {
    printk(KERN_EMERG DEVICE_NAME " demo init.\r\n");
  }
  
  return 0;
}

static void __exit demo_exit(void)
{
  unregister_chrdev(DRIVER_MAJOR, DEVICE_NAME);
  printk(KERN_EMERG DEVICE_NAME " demo exit.\r\n");
}

module_init(demo_init);
module_exit(demo_exit);
MODULE_LICENSE("GPL");

