#!/bin/sh
# 等号两边不能有空格
module="scull"
device="scull"
mode="666"
group=0

function load() {
    # insmode用来安装.ko驱动
    insmod ./$module.ko $* || exit 1

    # 删除/dev/scull0 ~ /dev/scull2这几个设备
    rm -f /dev/${device}[0-2]

    # 从/proc/devices文件中提取设备的主设备编号
    major=$(awk -v device="$device" '$2==device {print $1}' /proc/devices)
    # 在/dev文件夹下创建设备
    mknod /dev/${device}0 c $major 0
    mknod /dev/${device}1 c $major 1
    mknod /dev/${device}2 c $major 2

    # 更改当前脚本的用户组
    chgrp $group /dev/$device[0-2]
    # 更改当前脚本的权限
    chmod $mode /dev/$device[0-2]
}

function unload() {
    rm -f /dev/${device}[0-2]
    rmmod $module || exit 1
}

arg=${1:-"load"}
case $arg in
    load)
        load ;;
    unload)
        unload ;;
    reload)
        ( unload )
        load
        ;;
    *)
        echo "Usage: $0 {load | unload | reload}"
        echo "Default is load"
        exit 1
        ;;
esac

