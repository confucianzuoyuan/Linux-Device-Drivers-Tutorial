设备树文件添加以下内容：

```
&i2c0 {
    status = "okay";
    ssd1306:ssd1306@3c {
        compatible = "oled12832,ssd1306";
        reg = <0x3c>;
    };
};
```

交叉编译命令

```sh
ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- make -C /home/zuoyuan/v3s-workspace/linux/ M=${PWD} modules
```

在开发板上执行

```sh
insmod ssd1306.ko
echo hello > /dev/oled
```