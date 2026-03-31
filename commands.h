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
void cmd_touch(char args[][MAX_ARG_LEN], int argc);
void cmd_ls(char args[][MAX_ARG_LEN], int argc);
void cmd_cat(char args[][MAX_ARG_LEN], int argc);
void cmd_micropen(char args[][MAX_ARG_LEN], int argc);

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
    {"touch", "create an empty file", cmd_touch},
    {"ls", "list every file", cmd_ls},
    {"cat", "print file content", cmd_cat},
    {"micropen", "tiny text editor", cmd_micropen},
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
    println(" ");
    println("Take me back back home ");
    sleep_sec(2);
    println("duong ve cung chang co xa");
    sleep_sec(2);
    println("dua em ve qua ba nga nam, nam nga ba la nha");
    sleep_sec(3);
    println("Take me back back home");
    sleep_sec(1);
    println("duong ve cung chang co xa");
    sleep_sec(2);
    println("dem khuya roi sao khong co ai dua em di ve nha?");
    sleep_sec(2);
    println(" ");

}

void cmd_touch(char args[][MAX_ARG_LEN], int argc) {
    (void)args; (void)argc; 
    if (argc == 0) { println("Usage: touch <filename>"); return; }
    if (fs_write(args[0], "") < 0) {
        println("Failed to create file");
    } else {
        println("File created successfully");
    }
}

void cmd_ls(char args[][MAX_ARG_LEN], int argc) {
    (void)args; (void)argc; 
    fs_list();
}

void cmd_cat(char args[][MAX_ARG_LEN], int argc) {
    if (argc == 0) { println("Usage: cat <filename>"); return; }
    char data[FILE_SIZE];
    if (fs_read(args[0], data, FILE_SIZE) < 0) {
        println("File not found");
        return;
    }
    println(data);
}

static int mp_clamp(int v, int lo, int hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static int mp_index_to_col(const char* text, int len, int idx, int width) {
    int col = 0;
    idx = mp_clamp(idx, 0, len);
    for (int i = 0; i < idx; i++) {
        if (text[i] == '\n') col = 0;
        else {
            col++;
            if (col >= width) col = 0;
        }
    }
    return col;
}

static int mp_cursor_row(const char* text, int len, int idx, int width) {
    int row = 0;
    int col = 0;
    idx = mp_clamp(idx, 0, len);
    for (int i = 0; i < idx; i++) {
        if (text[i] == '\n') {
            row++;
            col = 0;
        } else {
            col++;
            if (col >= width) {
                row++;
                col = 0;
            }
        }
    }
    return row;
}

static int mp_move_up(const char* text, int len, int idx, int width) {
    int target_col = mp_index_to_col(text, len, idx, width);
    int line_start = idx;
    while (line_start > 0 && text[line_start - 1] != '\n') line_start--;
    if (line_start == 0) return idx;
    int prev_end = line_start - 1;
    int prev_start = prev_end;
    while (prev_start > 0 && text[prev_start - 1] != '\n') prev_start--;
    int prev_len = prev_end - prev_start;
    int col = mp_clamp(target_col, 0, prev_len);
    return prev_start + col;
}

static int mp_move_down(const char* text, int len, int idx, int width) {
    int target_col = mp_index_to_col(text, len, idx, width);
    int line_end = idx;
    while (line_end < len && text[line_end] != '\n') line_end++;
    if (line_end >= len) return idx;
    int next_start = line_end + 1;
    int next_end = next_start;
    while (next_end < len && text[next_end] != '\n') next_end++;
    int next_len = next_end - next_start;
    int col = mp_clamp(target_col, 0, next_len);
    return next_start + col;
}

static void mp_draw(const char* filename, const char* text, int len, int cursor, int* row_offset) {
    const int editor_rows = 22;
    const int editor_cols = 80;
    int cur_row = mp_cursor_row(text, len, cursor, editor_cols);

    if (cur_row < *row_offset) *row_offset = cur_row;
    if (cur_row >= *row_offset + editor_rows) *row_offset = cur_row - editor_rows + 1;
    if (*row_offset < 0) *row_offset = 0;

    clear_screen();
    print("micropen: ");
    println(filename);
    println("^S save | ^Q quit | arrows move");

    int row = 0;
    int col = 0;
    int logical_row = 0;
    for (int i = 0; i < len; i++) {
        char ch = text[i];
        if (logical_row >= *row_offset && row < editor_rows) {
            if (ch == '\n') {
                while (col < editor_cols) { vga_putchar(' '); col++; }
            } else {
                vga_putchar(ch);
                col++;
                if (col >= editor_cols) {
                    row++;
                    col = 0;
                    continue;
                }
            }
        }

        if (ch == '\n' || col >= editor_cols) {
            if (ch != '\n' && col >= editor_cols) {
                // already advanced row for wrap path
            } else {
                if (logical_row >= *row_offset && row < editor_rows) row++;
                col = 0;
            }
            logical_row++;
        }
        if (row >= editor_rows) break;
    }

    while (row < editor_rows) {
        while (col < editor_cols) { vga_putchar(' '); col++; }
        row++;
        col = 0;
    }

    int cur_col = mp_index_to_col(text, len, cursor, editor_cols);
    int draw_row = cur_row - *row_offset + 2; // two header lines
    if (draw_row >= 2 && draw_row < 2 + editor_rows) {
        move_cursor((uint32_t)cur_col, (uint32_t)draw_row);
    } else {
        move_cursor(0, 2);
    }
}

void cmd_micropen(char args[][MAX_ARG_LEN], int argc) {
    if (argc == 0) { println("Usage: micropen <filename>"); return; }

    char text[FILE_SIZE];
    int len = 0;
    int cursor = 0;
    int row_offset = 0;
    bool ctrl_pressed = false;

    if (fs_read(args[0], text, FILE_SIZE) < 0) {
        text[0] = 0;
    }
    while (text[len] && len < FILE_SIZE - 1) len++;
    cursor = len;

    show_cursor();
    while (1) {
        mp_draw(args[0], text, len, cursor, &row_offset);
        uint8_t sc = read_scancode();

        if (sc == 0xE0) {
            uint8_t ext = read_scancode();
            if (ext == 0x1D) { ctrl_pressed = true; continue; }   // Right Ctrl down
            if (ext == 0x9D) { ctrl_pressed = false; continue; }  // Right Ctrl up
            if (ext & 0x80) continue;
            if (ext == 0x4B && cursor > 0) cursor--; // left
            else if (ext == 0x4D && cursor < len) cursor++; // right
            else if (ext == 0x48) cursor = mp_move_up(text, len, cursor, 80); // up
            else if (ext == 0x50) cursor = mp_move_down(text, len, cursor, 80); // down
            continue;
        }

        // Match read_char() behavior for Shift handling first.
        if (sc == LSHIFT || sc == RSHIFT) { shift_pressed = true; continue; }
        if (sc == LSHIFT_RELEASE || sc == RSHIFT_RELEASE) { shift_pressed = false; continue; }

        if (sc == 0x1D) { ctrl_pressed = true; continue; }   // Ctrl down
        if (sc == 0x9D) { ctrl_pressed = false; continue; }  // Ctrl up
        if (sc & 0x80) continue; // break code

        if (ctrl_pressed && sc == 0x1F) { // Ctrl+S
            text[len] = 0;
            if (fs_write(args[0], text) < 0) println("save failed");
            else println("saved");
            sleep_ms(150);
            continue;
        }
        if (ctrl_pressed && sc == 0x10) { // Ctrl+Q
            clear_screen();
            hide_cursor();
            println("Exited micropen");
            return;
        }

        if (sc == 0x0E) { // backspace
            if (cursor > 0) {
                for (int i = cursor - 1; i < len; i++) text[i] = text[i + 1];
                cursor--;
                len--;
            }
            continue;
        }
        if (sc == 0x1C) { // enter
            if (len < FILE_SIZE - 1) {
                for (int i = len; i >= cursor; i--) text[i + 1] = text[i];
                text[cursor] = '\n';
                cursor++;
                len++;
            }
            continue;
        }
        char c = 0;
        if (sc < sizeof(scancode_to_ascii_lower)) {
            c = shift_pressed ? scancode_to_ascii_upper[sc] : scancode_to_ascii_lower[sc];
        }
        if (!c) continue;

        if (len < FILE_SIZE - 1) {
            for (int i = len; i >= cursor; i--) text[i + 1] = text[i];
            text[cursor] = c;
            cursor++;
            len++;
        }
    }
}


#endif