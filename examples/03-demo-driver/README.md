```sh
# 将驱动程序交叉编译为arm架构下的驱动程序
$ ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- make -C /home/zuoyuan/v3s-workspace/linux/ M=${PWD} modules
# 将应用程序交叉编译为arm架构下的程序，`-static` 静态链接的编译方式
$ arm-linux-gnueabihf-gcc -static -o demo demo_app.c
$ scp demo_driver.ko root@ip地址:/root/
$ scp demo root@ip地址:/root/
```
