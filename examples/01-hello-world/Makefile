ifneq ($(KERNELRELEASE),)

module-objs := hello_world.o
obj-m := hello_world.o

CFLAGS_hello_world.o := -DDEBUG

else

KDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

.PHONY: modules
modules:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

.PHONY: clean
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

endif
