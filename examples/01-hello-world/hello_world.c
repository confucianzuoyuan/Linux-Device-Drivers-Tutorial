#include <linux/module.h>

static int __init m_init(void)
{
  printk(KERN_ALERT "Hello, World!\r\n");
  return 0;
}

static void __exit m_exit(void)
{
  printk(KERN_ALERT "Bye, World!\r\n");
}

module_init(m_init);
module_exit(m_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shakespeare");
MODULE_DESCRIPTION("hello world");
