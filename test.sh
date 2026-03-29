#!/bin/bash

# this script COMPILES and tests the OS, Which is only really usefull if you are going to make contributions or modify it. The repo already has the compiled image that anyone can use if they don't want to modify the OS.

mkdir -p binary

nasm -f bin tiny.asm -o binary/boot.bin || { echo "i said no"; exit 1; }
nasm -f bin kernel.asm -o binary/kernel.bin || { echo "net wide assembler looked at kernel.asm and said no"; exit 1; }

echo 1. compiled the BIG IMPORTANT boys like the bootloader and the root assembly kernel
echo " "

nasm -f bin driver_memscan.asm -o binary/driver_memscan.bin
nasm -f bin driver_test.asm -o binary/driver_test.bin

echo 2. compiled the assembly drivers
echo " "

nasm -f elf32 entry.asm -o binary/entry.o || { echo "net wide assembler looked at entry.asm and said NO."; exit 1; }

echo 3. compiled some still BIG boys
echo " "

gcc -m32 -ffreestanding -fno-builtin -nostdlib -fno-pic -O2 -c advancedkernel.c -o binary/advancedkernel.o || { echo "advanced kernel build failed noooooo"; exit 1; }

echo 4. compiled one of the BIG IMPORTANT C Kernel.
echo " "

ld -m elf_i386 -Ttext 0x50000 --oformat binary -o binary/advancedkernel.bin binary/entry.o binary/advancedkernel.o || { echo "the linker looked at the object files and said HELL NO."; exit 1; }

echo 5. linked the big boy C kernel.
echo " "

dd if=/dev/zero of=microos.img bs=512 count=2880 2>/dev/null
dd if=binary/boot.bin of=microos.img conv=notrunc bs=512 seek=0 2>/dev/null
dd if=binary/kernel.bin of=microos.img conv=notrunc bs=512 seek=1 2>/dev/null
dd if=binary/driver_memscan.bin of=microos.img conv=notrunc bs=512 seek=10 2>/dev/null
dd if=binary/driver_test.bin of=microos.img conv=notrunc bs=512 seek=20 2>/dev/null
dd if=binary/advancedkernel.bin of=microos.img conv=notrunc bs=512 seek=40 2>/dev/null

echo 6. Glued everything together into microos.img!!
echo " "

echo enjoy microOS!!!
qemu-system-i386 -drive format=raw,file=microos.img
