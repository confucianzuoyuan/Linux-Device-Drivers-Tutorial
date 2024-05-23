```sh
$ sudo apt-get install linux-headers-$(uname -r)
$ make
$ sudo insmod hello_world.ko
$ sudo rmmod hello_world.ko
$ sudo dmesg | tail -2
```
