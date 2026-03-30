#ifndef COMMANDS_H
#define COMMANDS_H

#include "vga.h"
#include <stdint.h>
#include <stdbool.h>
#include "Cdrivers/kernel/Ctestkernel.h"

#define MAX_ARGS     16
#define MAX_ARG_LEN  64

void cmd_help(char args[][MAX_ARG_LEN], int argc);
void cmd_clear(char args[][MAX_ARG_LEN], int argc);
void cmd_meminfo(char args[][MAX_ARG_LEN], int argc);
void cmd_echo(char args[][MAX_ARG_LEN], int argc);
void cmd_color(char args[][MAX_ARG_LEN], int argc);
void graphicscardtestaaagh(char args[][MAX_ARG_LEN], int argc);
void cmd_jebus_easteregghaha(char args[][MAX_ARG_LEN], int argc);
void cmd_strbuffertest(char args[][MAX_ARG_LEN], int argc);
void sysfetch(char args[][MAX_ARG_LEN], int argc);
void lowcortisolsonglyrics(char args[][MAX_ARG_LEN], int argc);

typedef struct {
    const char* name;
    const char* description;
    void (*fn)(char args[][MAX_ARG_LEN], int argc);
} Command;

static Command command_table[] = {
    {"help",    "show this help menu",   cmd_help},
    {"clear",   "clear the screen",      cmd_clear},
    {"meminfo", "show available RAM",    cmd_meminfo},
    {"echo",    "print arguments",       cmd_echo},
    {"color",   "change text color 0-F", cmd_color},
    {"GPUtest", "small graphics tester", graphicscardtestaaagh},
    {"Ckerneldrivers_inittest", "Test kerneldriver that does nothing", Ckerneltestdriver_runnow},
    {"jebusishere", "Who is jebus", cmd_jebus_easteregghaha},
    {"strbuffertest", "test if buffers work or cry", cmd_strbuffertest},
    {"microfetch", "show system information", sysfetch},
    {"lowcortisolsong", "take me back back home", lowcortisolsonglyrics},
    {0, 0, 0}
};

static bool str_equals(const char* a, const char* b) {
    while (*a && *b) {
        if (*a != *b) return false;
        a++; b++;
    }
    return *a == *b;
}

static int parse_input(const char* input, char* cmd, char args[][MAX_ARG_LEN]) {
    uint32_t i = 0;
    uint32_t c = 0;
    while (input[i] && input[i] != ' ') cmd[c++] = input[i++];
    cmd[c] = 0;
    int argc = 0;
    while (input[i] && argc < MAX_ARGS) {
        while (input[i] == ' ') i++;
        if (!input[i]) break;
        uint32_t j = 0;
        while (input[i] && input[i] != ' ' && j < MAX_ARG_LEN - 1) args[argc][j++] = input[i++];
        args[argc][j] = 0;
        argc++;
    }
    return argc;
}

void run_command(const char* input) {
    char cmd[64];
    char args[MAX_ARGS][MAX_ARG_LEN];
    int argc = parse_input(input, cmd, args);
    for (uint32_t i = 0; command_table[i].name; i++) {
        if (str_equals(cmd, command_table[i].name)) {
            command_table[i].fn(args, argc);
            return;
        }
    }
    print("Unknown command: ");
    println(cmd);
    println("Type 'help' for available commands");
}

void cmd_help(char args[][MAX_ARG_LEN], int argc) {
    (void)args; (void)argc;
    println("");
    println("Available commands:");
    println("--------------------");
    for (uint32_t i = 0; command_table[i].name; i++) {
        print("  ");
        print(command_table[i].name);
        print(" - ");
        println(command_table[i].description);
    }
    println("");
}

void graphicscardtestaaagh(char args[][MAX_ARG_LEN], int argc) { //AAAGHHHHHHH 
    (void)args; (void)argc;
    clear_screen();
    println("  annoying graphics card test and yes i gave up on drawing shapes");
    println("---------------------------------------------------------------------");

    for (int i = 0; i < 256; i++) {
        if (i == 7 || i == 8 || i == 9 || i == 10 || i == 13) {
            vga_putchar(' '); 
        } else {
            vga_putchar((char)i);
        }
        vga_putchar(' ');
        if ((i + 1) % 16 == 0) {
            vga_putchar('\n');
        }
    }

    println("\n------------------------------");
    println("test Complete");
}
void cmd_jebus_easteregghaha(char args[][MAX_ARG_LEN], int argc) {
    (void)args; (void)argc;
    println("NOOO JEBUS STOP NOOO HE IS GONNA EAT THE WORLDDD SOMEONE STOP HIMM");
}

void cmd_strbuffertest(char args[][MAX_ARG_LEN], int argc) {
    (void)args; (void)argc;
    char *buff = strdup("[*] Pass it worked!!!!");
    println(buff);
    free(buff);
}


void cmd_clear(char args[][MAX_ARG_LEN], int argc) {
    (void)args; (void)argc;
    clear_screen();
}

void cmd_meminfo(char args[][MAX_ARG_LEN], int argc) {
    (void)args; (void)argc;
    print_mem_info();
}

void cmd_echo(char args[][MAX_ARG_LEN], int argc) {
    for (int i = 0; i < argc; i++) {
        print(args[i]);
        if (i < argc - 1) print(" ");
    }
    println("");
}

void cmd_color(char args[][MAX_ARG_LEN], int argc) {
    if (argc == 0) { println("Usage: color <0-9 or A-F>"); return; }
    uint8_t c = args[0][0];
    if      (c >= '0' && c <= '9') c -= '0';
    else if (c >= 'A' && c <= 'F') c = c - 'A' + 10;
    else if (c >= 'a' && c <= 'f') c = c - 'a' + 10;
    else { println("Invalid color!"); return; }
    vga_color = c;
}

void sysfetch(char args[][MAX_ARG_LEN], int argc) { 
    (void)args; (void)argc;
    println("microfetch ");
    kprintf("Kernel: %s\n", "Advanced Mode");
    kprintf("CPU: %s\n", "x86_64");
    kprintf("Heap: %u bytes used, %u bytes free\n", heap_used(), heap_free());
    kprintf("VGA Text Mode: %ux%u\n", VGA_WIDTH, VGA_HEIGHT);
    println("");
    print_mem_info();
}

void lowcortisolsonglyrics(char args[][MAX_ARG_LEN], int argc) {
    (void)args; (void)argc;
    println("Take me back back home ");
    println("duong ve cung chang co xa");
    println("dua em ve qua ba nga nam, nam nga ba la nha");
    println("Take me back back home");
    println("duong ve cung chang co xa");
    println("dem khuya roi sao khong co ai dua em di ve nha?");

}

#endif