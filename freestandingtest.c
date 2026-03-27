#include "vga.h"

void kernel_main() { // this is the first freestanding C file ever made in microOS. It plays no role in it but we have to appreaciate it.
    const char* os_name = "MicroOS";
    int version = 2;
    
    println("Booting");
    kprintf("Welcome to %s version %d\n", os_name, version);
    kprintf("Kernel loaded at %x\n", 0x8000);
}