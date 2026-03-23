#include "vga.h"
#include "commands.h"

void kernel_main() {
    clear_screen();
    hide_cursor();
    println("");
    println("");
    println("    \xC9\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBB");
    println("    \xBA   MICRO OS                     \xBA");
    println("    \xBA   Codename: newC               \xBA");
    println("    \xBA   Advanced Mode                \xBA");
    println("    \xC8\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBC");
    println("       __________________________");
    println(" ");
    print("          ");
    print_mem_info();
    println("       __________________________");
    println("");

    char input[128];

    while (1) {
        print("> ");
        read_line(input, 128);
        if (input[0]) run_command(input);
    }
}