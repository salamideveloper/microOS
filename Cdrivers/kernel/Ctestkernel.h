#pragma once

#include "../../vga.h"

#ifndef CTESTKERNEL_H
#define CTESTKERNEL_H

void Ckerneltestdriver_runnow(char args[][64], int argc) {
    (void)args; (void)argc;

    char bytestring[] = "__abcdefghijklmnopqrstuvwxyz__";

    println("[*] Kernel driver");
    println("[Bytecount test:");
    kprintf("Should return [31] | %u\n", sizeof(bytestring));
    if (sizeof(bytestring) == 31) {
        println("[Bytecount, Healthy!]      | [✓]");
    }
}

#endif