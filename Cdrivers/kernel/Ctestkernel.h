#pragma once

#include "../../vga.h"

#ifndef CTESTKERNEL_H
#define CTESTKERNEL_H

void Ckerneltestdriver_runnow(char args[][64], int argc) {
    (void)args; (void)argc;
    println("if this works im gonna touch myself");
}

#endif