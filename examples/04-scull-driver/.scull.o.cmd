savedcmd_/home/zuoyuan/Linux-Device-Drivers-Tutorial/examples/04-scull-driver/scull.o := ld -m elf_x86_64 -z noexecstack --no-warn-rwx-segments   -r -o /home/zuoyuan/Linux-Device-Drivers-Tutorial/examples/04-scull-driver/scull.o @/home/zuoyuan/Linux-Device-Drivers-Tutorial/examples/04-scull-driver/scull.mod  ; ./tools/objtool/objtool --hacks=jump_label --hacks=noinstr --hacks=skylake --ibt --orc --retpoline --rethunk --sls --static-call --uaccess --prefix=16  --link  --module /home/zuoyuan/Linux-Device-Drivers-Tutorial/examples/04-scull-driver/scull.o

/home/zuoyuan/Linux-Device-Drivers-Tutorial/examples/04-scull-driver/scull.o: $(wildcard ./tools/objtool/objtool)
