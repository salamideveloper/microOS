#!/bin/bash

# Assembly bootloader
nasm -f bin tiny.asm -o boot.bin || { echo "bootloader build failed nooo"; exit 1; }

# Assembly kernel (real mode CLI)
nasm -f bin kernel.asm -o kernel.bin || { echo "assembly kernel build failed noooooo"; exit 1; }

# Drivers
nasm -f bin driver_memscan.asm -o driver_memscan.bin
nasm -f bin driver_test.asm -o driver_test.bin

# C kernel entry point
nasm -f elf32 entry.asm -o entry.o || { echo "entry.asm build failed"; exit 1; }

# Advanced C kernel
gcc -m32 -ffreestanding -fno-builtin -nostdlib -fno-pic -O2 -c advancedkernel.c -o advancedkernel.o || { echo "advanced kernel build failed noooooo"; exit 1; }
# Link C kernel
ld -m elf_i386 -Ttext 0x50000 --oformat binary -o advancedkernel.bin entry.o advancedkernel.o || { echo "linker said no"; exit 1; }

# Build disk image
dd if=/dev/zero of=microos.img bs=512 count=2880 2>/dev/null
dd if=boot.bin of=microos.img conv=notrunc bs=512 seek=0 2>/dev/null
dd if=kernel.bin of=microos.img conv=notrunc bs=512 seek=1 2>/dev/null
dd if=driver_memscan.bin of=microos.img conv=notrunc bs=512 seek=10 2>/dev/null
dd if=driver_test.bin of=microos.img conv=notrunc bs=512 seek=20 2>/dev/null
dd if=advancedkernel.bin of=microos.img conv=notrunc bs=512 seek=40 2>/dev/null

# Run in QEMU
qemu-system-i386 -drive format=raw,file=microos.img