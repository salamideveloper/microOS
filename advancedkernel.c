#include "vga.h"
#include "commands.h"

void kernel_main() {
    pit_init(); // PLEASE FUCKING WORK FOR FUCKING ONCE
    fs_init();
    fs_load();
    makefilesys();
    clear_screen();
    show_cursor();
    println("");
    println("");
    println("    \xC9\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBB");
    println("    \xBA   MICRO OS                     \xBA");
    println("    \xBA   Codename: Hazel              \xBA");
    println("    \xBA   Advanced Mode                \xBA");
    println("    \xC8\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBC");
    println("       __________________________");
    println(" ");
    print("          ");
    print_mem_info();
    println("          C kernel Terminal");
    println("       __________________________");
    println("");

    if (!fs_exists("experienced__m")) {
        fs_write("experienced__m", "yes");
        println("It seems this is your first time running microOS");
        println("Your default 'supr' password is 'root'. ");
        println("You can change it with the 'setpassw <newpassword>' command. ");
        println("THIS IS THE ONLY TIME YOU WILL BE ABLE TO CHANGE THE PASSWORD!!!!");
        fs_intwrite("passwordeditable", 1);
    } else {
        fs_intwrite("passwordeditable", 0);
    }

    char input[1024];

    while (1) {
        print("> ");
        read_line(input, 128);
        if (input[0]) run_command(input);
    }
}