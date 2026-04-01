#ifndef FILESYS_H
#define FILESYS_H

#include "vga.h"

static void makefilesys() {
    if (!fs_exists("userfile")) {
        fs_write("userfile", "root");
    }

    if (!fs_exists("userfile_passw")) {
        fs_write_hidden("userfile_passw", "root");
    }
}

static void setuser(const char* name) {
    fs_write("userfile", name);
}

static void kernelsetpassw(const char* passw) {
    fs_write_hidden("userfile_passw", passw);
}

static void setpassw(char args[][MAX_ARG_LEN], int argc) {
    int32_t editable = 0;
    if (fs_intread("passwordeditable", &editable) != 0 || editable != 1) {
        println("Password is not editable.");
        return;
    }

    if (argc < 1) {
        println("Usage: setpassw <newpassword>");
        return;
    }

    kernelsetpassw(args[0]);
}

static void kernelprintpassw() {
    char buf[FILE_SIZE];
    if (fs_read("userfile_passw", buf, sizeof(buf)) < 0) {
        println("Could not read password");
        return;
    }

    print("Your password is: ");
    println(buf);
}

#endif