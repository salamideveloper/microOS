#!/bin/bash

mkdir -p binary

nasm -f bin tiny.asm -o binary/boot.bin || { echo "boot failed"; exit 1; }
nasm -f bin kernel.asm -o binary/kernel.bin || { echo "kernel failed"; exit 1; }

nasm -f elf32 entry.asm -o binary/entry.o || { echo "entry failed"; exit 1; }

gcc -m32 -ffreestanding -fno-builtin -nostdlib -fno-pic -O2 \
-c advancedkernel.c -o binary/advancedkernel.o || { echo "C kernel failed"; exit 1; }

nasm -f elf32 more_asem/diskwrite.asm -o binary/diskwrite.o || { echo "disk asm failed"; exit 1; }

ld -m elf_i386 -Ttext 0x50000 --oformat binary \
-o binary/advancedkernel.bin \
binary/entry.o \
binary/advancedkernel.o \
binary/diskwrite.o || { echo "link failed"; exit 1; }

echo "the BIG important broskis compiled."

nasm -f bin driver_memscan.asm -o binary/driver_memscan.bin || exit 1
nasm -f bin driver_test.asm -o binary/driver_test.bin || exit 1

echo "2. drivers built"

echo "the entry assembly module compiled lets goo"

echo "yayy we compiled the C kernel"

echo "yeahh kernel linked"

echo "just the disk image now."

dd if=/dev/zero of=microos.img bs=512 count=2880 2>/dev/null

dd if=binary/boot.bin of=microos.img conv=notrunc bs=512 seek=0 2>/dev/null
dd if=binary/kernel.bin of=microos.img conv=notrunc bs=512 seek=1 2>/dev/null

dd if=binary/driver_memscan.bin of=microos.img conv=notrunc bs=512 seek=10 2>/dev/null
dd if=binary/driver_test.bin of=microos.img conv=notrunc bs=512 seek=20 2>/dev/null

dd if=binary/advancedkernel.bin of=microos.img conv=notrunc bs=512 seek=40 2>/dev/null

echo "No fails lets gooo"

echo "Run??? (y/n)"
read answer

if [[ "$answer" == "y" ]]; then
    qemu-system-i386 -drive format=raw,file=microos.img
else
    echo "ok bye"
fi