ifneq ($(KERNELRELEASE),)
# `-std=gnu99` 表示使用的c语言语法是c99
    ccflags-y := -std=gnu99 -DENABLE_DEBUG
# 要编译main.c, fops.c
    scull-objs := main.o fops.o
# obj-m 表示编译到 scull.o 模块，obj-m的意思是编译出scull.ko文件作为驱动程序
    obj-m := scull.o
else
# KERNELDIR表示的是内核源码头文件的目录
# `uname -r` 返回的是当前机器内核的版本号
    KERNELDIR ?= /lib/modules/$(shell uname -r)/build
# 当前目录
    PWD := $(shell pwd)

.PHONY: clean
modules:
# 编译模块
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif

.PHONY: clean
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

CFLAGS_main.o := -DDEBUG
CFLAGS_fops.o := -DDEBUG
