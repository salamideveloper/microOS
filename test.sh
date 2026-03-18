#!/bin/bash
nasm -f bin tiny.asm -o boot.bin || { echo "the bootloader build failed nooo"; exit 1; }
nasm -f bin kernel.asm -o kernel.bin || { echo "the kernel build failed noooooo"; exit 1; }
nasm -f bin driver_memscan.asm -o driver_memscan.bin
nasm -f bin driver_test.asm -o driver_test.bin

dd if=/dev/zero of=microos.img bs=512 count=2880 2>/dev/null
dd if=boot.bin of=microos.img conv=notrunc bs=512 seek=0 2>/dev/null
dd if=kernel.bin of=microos.img conv=notrunc bs=512 seek=1 2>/dev/null
dd if=driver_memscan.bin of=microos.img conv=notrunc bs=512 seek=10 2>/dev/null
dd if=driver_test.bin of=microos.img conv=notrunc bs=512 seek=20 2>/dev/null

qemu-system-i386 -drive format=raw,file=microos.img
