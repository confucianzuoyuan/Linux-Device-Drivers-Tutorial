ifneq ($(KERNELRELEASE),)

ccflags-y := -std=gnu99
module_parameters-objs := main.o
obj-m := module_parameters.o

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
