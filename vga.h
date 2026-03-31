#ifndef VGA_H
#define VGA_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define VGA_MEMORY     0xB8000
#define VGA_GFX_MEMORY 0xA0000  
#define VGA_WIDTH      80
#define VGA_HEIGHT     25

static uint8_t* vga_buffer = (uint8_t*)VGA_GFX_MEMORY;
#define HEAP_START   0x200000
#define HEAP_SIZE    0x400000

#define LSHIFT         0x2A
#define RSHIFT         0x36
#define LSHIFT_RELEASE 0xAA
#define RSHIFT_RELEASE 0xB6

volatile uint32_t ticks = 0; // makes sure the CPU doesnt kill itself

static int32_t cursor_x = 0;
static int32_t cursor_y = 0;
uint8_t vga_color = 0x07;
static bool shift_pressed = false;

static uint8_t* heap_ptr = (uint8_t*)HEAP_START;
static uint8_t* heap_end = (uint8_t*)(HEAP_START + HEAP_SIZE);

typedef struct AllocBlock {
    uint32_t size;
    bool free;
    struct AllocBlock* next;
} AllocBlock;

static AllocBlock* heap_head = (AllocBlock*)HEAP_START;
static bool heap_initialized = false;

void heap_init() {
    heap_head->size = HEAP_SIZE - sizeof(AllocBlock);
    heap_head->free = true;
    heap_head->next = 0;
    heap_initialized = true;
}

void* malloc(uint32_t size) {
    if (!heap_initialized) heap_init();
    AllocBlock* block = heap_head;
    while (block) {
        if (block->free && block->size >= size) {
            if (block->size > size + sizeof(AllocBlock)) {
                AllocBlock* newblock = (AllocBlock*)((uint8_t*)block + sizeof(AllocBlock) + size);
                newblock->size = block->size - size - sizeof(AllocBlock);
                newblock->free = true;
                newblock->next = block->next;
                block->next = newblock;
                block->size = size;
            }
            block->free = false;
            return (void*)((uint8_t*)block + sizeof(AllocBlock));
        }
        block = block->next;
    }
    return 0;
}

void free(void* ptr) {
    if (!ptr) return;
    AllocBlock* block = (AllocBlock*)((uint8_t*)ptr - sizeof(AllocBlock));
    block->free = true;
    AllocBlock* cur = heap_head;
    while (cur && cur->next) {
        if (cur->free && cur->next->free) {
            cur->size += sizeof(AllocBlock) + cur->next->size;
            cur->next = cur->next->next;
        } else {
            cur = cur->next;
        }
    }
}

void* realloc(void* ptr, uint32_t size) {
    if (!ptr) return malloc(size);
    AllocBlock* block = (AllocBlock*)((uint8_t*)ptr - sizeof(AllocBlock));
    if (block->size >= size) return ptr;
    void* newptr = malloc(size);
    if (!newptr) return 0;
    uint8_t* src = (uint8_t*)ptr;
    uint8_t* dst = (uint8_t*)newptr;
    for (uint32_t i = 0; i < block->size; i++) dst[i] = src[i];
    free(ptr);
    return newptr;
}

void* calloc(uint32_t count, uint32_t size) {
    void* ptr = malloc(count * size);
    if (!ptr) return 0;
    uint8_t* p = (uint8_t*)ptr;
    for (uint32_t i = 0; i < count * size; i++) p[i] = 0;
    return ptr;
}

uint32_t heap_free() {
    uint32_t total = 0;
    AllocBlock* block = heap_head;
    while (block) {
        if (block->free) total += block->size;
        block = block->next;
    }
    return total;
}

uint32_t heap_used() {
    uint32_t total = 0;
    AllocBlock* block = heap_head;
    while (block) {
        if (!block->free) total += block->size;
        block = block->next;
    }
    return total;
}

uint32_t strlen(const char* str) {
    uint32_t len = 0;
    while (str[len]) len++;
    return len;
}

bool streq(const char* a, const char* b) {
    while (*a && *b) {
        if (*a != *b) return false;
        a++; b++;
    }
    return *a == *b;
}

char* strcpy(char* dst, const char* src) {
    char* d = dst;
    while ((*d++ = *src++));
    return dst;
}

char* strcat(char* dst, const char* src) {
    char* d = dst;
    while (*d) d++;
    while ((*d++ = *src++));
    return dst;
}

int32_t strcmp(const char* a, const char* b) {
    while (*a && *b && *a == *b) { a++; b++; }
    return (uint8_t)*a - (uint8_t)*b;
}

char* strncpy(char* dst, const char* src, uint32_t n) {
    uint32_t i = 0;
    while (i < n && src[i]) { dst[i] = src[i]; i++; }
    while (i < n) { dst[i++] = 0; }
    return dst;
}

char* strchr(const char* str, char c) {
    while (*str) {
        if (*str == c) return (char*)str;
        str++;
    }
    return 0;
}

void* memset(void* ptr, uint8_t value, uint32_t size) {
    uint8_t* p = (uint8_t*)ptr;
    for (uint32_t i = 0; i < size; i++) p[i] = value;
    return ptr;
}

void* memcpy(void* dst, const void* src, uint32_t size) {
    uint8_t* d = (uint8_t*)dst;
    const uint8_t* s = (const uint8_t*)src;
    for (uint32_t i = 0; i < size; i++) d[i] = s[i];
    return dst;
}

void* memmove(void* dst, const void* src, uint32_t size) {
    uint8_t* d = (uint8_t*)dst;
    const uint8_t* s = (const uint8_t*)src;
    if (d < s) {
        for (uint32_t i = 0; i < size; i++) d[i] = s[i];
    } else {
        for (int32_t i = size - 1; i >= 0; i--) d[i] = s[i];
    }
    return dst;
}

int32_t memcmp(const void* a, const void* b, uint32_t size) {
    const uint8_t* pa = (const uint8_t*)a;
    const uint8_t* pb = (const uint8_t*)b;
    for (uint32_t i = 0; i < size; i++) {
        if (pa[i] != pb[i]) return pa[i] - pb[i];
    }
    return 0;
}

int32_t atoi(const char* str) {
    int32_t result = 0;
    bool negative = false;
    if (*str == '-') { negative = true; str++; }
    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }
    return negative ? -result : result;
}

void itoa(int32_t n, char* buf) {
    if (n == 0) { buf[0] = '0'; buf[1] = 0; return; }
    bool negative = n < 0;
    if (negative) n = -n;
    char tmp[32];
    int32_t i = 0;
    while (n > 0) { tmp[i++] = '0' + (n % 10); n /= 10; }
    if (negative) tmp[i++] = '-';
    int32_t j = 0;
    for (int32_t k = i - 1; k >= 0; k--) buf[j++] = tmp[k];
    buf[j] = 0;
}

void utoa(uint32_t n, char* buf) {
    if (n == 0) { buf[0] = '0'; buf[1] = 0; return; }
    char tmp[32];
    int32_t i = 0;
    while (n > 0) { tmp[i++] = '0' + (n % 10); n /= 10; }
    int32_t j = 0;
    for (int32_t k = i - 1; k >= 0; k--) buf[j++] = tmp[k];
    buf[j] = 0;
}

void htoa(uint32_t n, char* buf) {
    const char hex[] = "0123456789ABCDEF";
    buf[0] = '0'; buf[1] = 'x';
    int32_t i = 2;
    bool leading = true;
    for (int32_t s = 28; s >= 0; s -= 4) {
        uint8_t nibble = (n >> s) & 0xF;
        if (nibble != 0) leading = false;
        if (!leading) buf[i++] = hex[nibble];
    }
    if (leading) buf[i++] = '0';
    buf[i] = 0;
}

bool isdigit(char c) { return c >= '0' && c <= '9'; }
bool isalpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
bool isalnum(char c) { return isdigit(c) || isalpha(c); }
bool isspace(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }
bool isupper(char c) { return c >= 'A' && c <= 'Z'; }
bool islower(char c) { return c >= 'a' && c <= 'z'; }
char toupper(char c) { return islower(c) ? c - 32 : c; }
char tolower(char c) { return isupper(c) ? c + 32 : c; }

int32_t abs(int32_t n) { return n < 0 ? -n : n; }
int32_t min(int32_t a, int32_t b) { return a < b ? a : b; }
int32_t max(int32_t a, int32_t b) { return a > b ? a : b; }
uint32_t umin(uint32_t a, uint32_t b) { return a < b ? a : b; }
uint32_t umax(uint32_t a, uint32_t b) { return a > b ? a : b; }

void io_outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %1, %0" : : "dN"(port), "a"(val));
}

uint8_t io_inb(uint16_t port) {
    uint8_t val;
    __asm__ volatile ("inb %1, %0" : "=a"(val) : "dN"(port));
    return val;
}

void io_outw(uint16_t port, uint16_t val) {
    __asm__ volatile ("outw %1, %0" : : "dN"(port), "a"(val));
}

uint16_t io_inw(uint16_t port) {
    uint16_t val;
    __asm__ volatile ("inw %1, %0" : "=a"(val) : "dN"(port));
    return val;
}

void cpu_halt() { __asm__ volatile ("hlt"); }
void cpu_cli()  { __asm__ volatile ("cli"); }
void cpu_sti()  { __asm__ volatile ("sti"); }
void cpu_nop()  { __asm__ volatile ("nop"); }

uint32_t cpu_eflags() {
    uint32_t flags;
    __asm__ volatile ("pushfl; popl %0" : "=r"(flags));
    return flags;
}

void clear_screen() {
    volatile uint8_t* vga = (volatile uint8_t*)VGA_MEMORY;
    for (int32_t i = 0; i < VGA_WIDTH * VGA_HEIGHT * 2; i += 2) {
        vga[i]     = ' ';
        vga[i + 1] = vga_color;
    }
    cursor_x = 0;
    cursor_y = 0;
}

void vga_putchar(char c) {
    volatile uint8_t* vga = (volatile uint8_t*)VGA_MEMORY;
    if (c == '\n') { cursor_x = 0; cursor_y++; return; }
    if (c == '\r') { cursor_x = 0; return; }
    if (c == '\t') { cursor_x += 4; return; }
    if (cursor_y >= VGA_HEIGHT) {
        for (int32_t i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH * 2; i++)
            vga[i] = vga[i + VGA_WIDTH * 2];
        for (int32_t i = (VGA_HEIGHT - 1) * VGA_WIDTH * 2; i < VGA_HEIGHT * VGA_WIDTH * 2; i += 2) {
            vga[i]     = ' ';
            vga[i + 1] = vga_color;
        }
        cursor_y = VGA_HEIGHT - 1;
    }
    uint32_t index = (cursor_y * VGA_WIDTH + cursor_x) * 2;
    vga[index]     = (uint8_t)c;
    vga[index + 1] = vga_color;
    cursor_x++;
    if (cursor_x >= VGA_WIDTH) { cursor_x = 0; cursor_y++; }
}

void print(const char* str) { while (*str) vga_putchar(*str++); }
void println(const char* str) { print(str); vga_putchar('\n'); }

void print_int(int32_t n) {
    char buf[32];
    itoa(n, buf);
    print(buf);
}

void print_uint(uint32_t n) {
    char buf[32];
    utoa(n, buf);
    print(buf);
}

void print_hex(uint32_t n) {
    char buf[32];
    htoa(n, buf);
    print(buf);
}

void print_bool(bool b) { println(b ? "true" : "false"); }
void vga_setcolor(uint8_t color) { vga_color = color; }

void kprintf(const char* fmt, ...) {
    uint32_t* args = (uint32_t*)(&fmt + 1);
    int32_t arg_index = 0;
    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 's': print((const char*)args[arg_index++]); break;
                case 'd': print_int((int32_t)args[arg_index++]); break;
                case 'u': print_uint(args[arg_index++]); break;
                case 'x': print_hex(args[arg_index++]); break;
                case 'c': vga_putchar((char)args[arg_index++]); break;
                case 'b': print_bool((bool)args[arg_index++]); break;
                case '%': vga_putchar('%'); break;
            }
        } else {
            vga_putchar(*fmt);
        }
        fmt++;
    }
}

void print_mem_info() {
    uint8_t low  = io_inb(0x71);
    io_outb(0x70, 0x31);
    uint8_t high = io_inb(0x71);
    uint32_t mb  = (((uint32_t)(high << 8) | low) / 1024) + 1;
    print("Available RAM: ");
    print_uint(mb);
    println(" MB");
}

void print_heap_info() {
    print("Heap used: ");
    print_uint(heap_used());
    print(" bytes  |  Heap free: ");
    print_uint(heap_free());
    println(" bytes");
}

static char scancode_to_ascii_lower[] = {
    0, 0, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,'\\','z','x','c','v','b','n','m',',','.','/',0,
    '*',0,' '
};

static char scancode_to_ascii_upper[] = {
    0, 0, '!','@','#','$','%','^','&','*','(',')','_','+','\b',
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
    0,'A','S','D','F','G','H','J','K','L',':','"','~',
    0,'|','Z','X','C','V','B','N','M','<','>','?',0,
    '*',0,' '
};

uint8_t read_scancode() {
    uint8_t status, scancode;
    do { status = io_inb(0x64); } while (!(status & 0x01));
    scancode = io_inb(0x60);
    return scancode;
}

char read_char() {
    uint8_t scancode;
    while (1) {
        scancode = read_scancode();
        if (scancode == LSHIFT || scancode == RSHIFT) { shift_pressed = true; continue; }
        if (scancode == LSHIFT_RELEASE || scancode == RSHIFT_RELEASE) { shift_pressed = false; continue; }
        if (scancode & 0x80) continue;
        if (scancode < sizeof(scancode_to_ascii_lower)) {
            char c = shift_pressed ? scancode_to_ascii_upper[scancode] : scancode_to_ascii_lower[scancode];
            if (c) return c;
        }
    }
}

void read_line(char* buffer, uint32_t max) {
    uint32_t i = 0;
    while (1) {
        char c = read_char();
        if (c == '\n') { buffer[i] = 0; vga_putchar('\n'); return; }
        if (c == '\b') {
            if (i > 0) {
                i--;
                cursor_x--;
                uint32_t index = (cursor_y * VGA_WIDTH + cursor_x) * 2;
                volatile uint8_t* vga = (volatile uint8_t*)VGA_MEMORY;
                vga[index] = ' ';
            }
            continue;
        }
        if (i < max - 1) { buffer[i++] = c; vga_putchar(c); }
    }
}

void draw_rect(int x, int y, int width, int height) {
    volatile uint8_t* vga = (volatile uint8_t*)VGA_MEMORY;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            uint32_t index = ((y + i) * VGA_WIDTH + (x + j)) * 2;
            vga[index] = 0xDB; // i have no idea what to put here now this is just shit but it works atleast but its so shittts

        }
    }
}

void draw_txt(int x, int y, char* text) {

}

void hide_cursor() {
    io_outb(0x3D4, 0x0A);
    io_outb(0x3D5, 0x20);
}

void show_cursor() {
    io_outb(0x3D4, 0x0A); io_outb(0x3D5, 0x00);
    io_outb(0x3D4, 0x0B); io_outb(0x3D5, 0x0F);
}

void move_cursor(uint32_t x, uint32_t y) {
    uint16_t pos = y * VGA_WIDTH + x;
    io_outb(0x3D4, 0x0F); io_outb(0x3D5, (uint8_t)(pos & 0xFF));
    io_outb(0x3D4, 0x0E); io_outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

char* strdup(const char* s) { // so i can mallocate a string without using like 5 lines of code.
    uint32_t len = strlen(s) + 1;
    char* buf = malloc(len);
    memcpy(buf, s, len);
    return buf;
}

#define PIT_FREQ 1193182 // il just put these here instead of the top 
#define HZ 1000

void pit_init() {
    io_outb(0x43, 0x34);
    io_outb(0x40, 0xFF);
    io_outb(0x40, 0xFF);
}

uint16_t pit_read() {
    io_outb(0x43, 0x00);
    uint8_t lo = io_inb(0x40);
    uint8_t hi = io_inb(0x40);
    return (uint16_t)(hi << 8) | lo;
}

void sleep_ms(uint32_t ms) {
    uint32_t ticks_needed = (uint32_t)(1193182UL * ms / 1000);
    uint32_t ticks_counted = 0;
    uint16_t last = pit_read();
    while (ticks_counted < ticks_needed) {
        uint16_t current = pit_read();
        uint16_t diff;
        if (last >= current) diff = last - current;
        else diff = last + (0xFFFF - current);
        ticks_counted += diff;
        last = current;
    }
}

void sleep_sec(uint32_t seconds) { sleep_ms(seconds * 1000); }


// AAAAAAAAAGHHH DUMB FILESYSTEM AGHHHHHHHHHHH

#define MAX_FILES 64
#define NAME_LEN 32
#define FILE_SIZE 512
#define FS_SECTOR_SIZE 512
#define FS_LBA_START 200
#define FS_MAGIC 0x3153464Du

typedef struct {
    char name[NAME_LEN];
    char data[FILE_SIZE];
    int used;
} File;

static File fs[MAX_FILES];

typedef struct {
    uint32_t magic;
    uint32_t version;
    File files[MAX_FILES];
} FsDiskImage;

#define FS_SECTOR_COUNT ((sizeof(FsDiskImage) + FS_SECTOR_SIZE - 1) / FS_SECTOR_SIZE)
static FsDiskImage fs_disk_image;

static int ata_wait_not_busy() {
    for (uint32_t i = 0; i < 1000000; i++) {
        uint8_t status = io_inb(0x1F7);
        if (!(status & 0x80)) return 0;
    }
    return -1;
}

static int ata_wait_data_ready() {
    for (uint32_t i = 0; i < 1000000; i++) {
        uint8_t status = io_inb(0x1F7);
        if (status & 0x01) return -1;     // ERR
        if (!(status & 0x80) && (status & 0x08)) return 0; // !BSY && DRQ
    }
    return -1;
}

static int disk_read_sector(uint32_t lba, uint8_t* buffer) {
    if (ata_wait_not_busy() < 0) return -1;

    io_outb(0x1F6, 0xE0 | ((lba >> 24) & 0x0F));
    io_outb(0x1F2, 1);
    io_outb(0x1F3, (uint8_t)(lba & 0xFF));
    io_outb(0x1F4, (uint8_t)((lba >> 8) & 0xFF));
    io_outb(0x1F5, (uint8_t)((lba >> 16) & 0xFF));
    io_outb(0x1F7, 0x20);

    if (ata_wait_data_ready() < 0) return -1;

    for (uint32_t i = 0; i < FS_SECTOR_SIZE / 2; i++) {
        uint16_t w = io_inw(0x1F0);
        buffer[i * 2] = (uint8_t)(w & 0xFF);
        buffer[i * 2 + 1] = (uint8_t)(w >> 8);
    }
    return 0;
}

static int disk_write_sector(uint32_t lba, const uint8_t* buffer) {
    if (ata_wait_not_busy() < 0) return -1;

    io_outb(0x1F6, 0xE0 | ((lba >> 24) & 0x0F));
    io_outb(0x1F2, 1);
    io_outb(0x1F3, (uint8_t)(lba & 0xFF));
    io_outb(0x1F4, (uint8_t)((lba >> 8) & 0xFF));
    io_outb(0x1F5, (uint8_t)((lba >> 16) & 0xFF));
    io_outb(0x1F7, 0x30);

    if (ata_wait_data_ready() < 0) return -1;

    for (uint32_t i = 0; i < FS_SECTOR_SIZE / 2; i++) {
        uint16_t w = (uint16_t)buffer[i * 2] | ((uint16_t)buffer[i * 2 + 1] << 8);
        io_outw(0x1F0, w);
    }

    io_outb(0x1F7, 0xE7); // flush cache
    return ata_wait_not_busy();
}

void fs_init() {
    for (int i = 0; i < MAX_FILES; i++)
        fs[i].used = 0;
}

int fs_save() {
    uint8_t sector[FS_SECTOR_SIZE];
    uint8_t* raw = (uint8_t*)&fs_disk_image;
    uint32_t total_bytes = sizeof(FsDiskImage);

    memset(&fs_disk_image, 0, sizeof(FsDiskImage));
    fs_disk_image.magic = FS_MAGIC;
    fs_disk_image.version = 1;
    memcpy(fs_disk_image.files, fs, sizeof(fs));

    for (uint32_t s = 0; s < FS_SECTOR_COUNT; s++) {
        uint32_t byte_offset = s * FS_SECTOR_SIZE;
        uint32_t remaining = (byte_offset < total_bytes) ? (total_bytes - byte_offset) : 0;
        uint32_t copy_len = remaining > FS_SECTOR_SIZE ? FS_SECTOR_SIZE : remaining;
        memset(sector, 0, FS_SECTOR_SIZE);
        if (copy_len) memcpy(sector, raw + byte_offset, copy_len);
        if (disk_write_sector(FS_LBA_START + s, sector) < 0) return -1;
    }

    return 0;
}

int fs_load() {
    uint8_t sector[FS_SECTOR_SIZE];
    uint8_t* raw = (uint8_t*)&fs_disk_image;
    uint32_t total_bytes = sizeof(FsDiskImage);

    memset(&fs_disk_image, 0, sizeof(FsDiskImage));
    for (uint32_t s = 0; s < FS_SECTOR_COUNT; s++) {
        uint32_t byte_offset = s * FS_SECTOR_SIZE;
        uint32_t remaining = (byte_offset < total_bytes) ? (total_bytes - byte_offset) : 0;
        uint32_t copy_len = remaining > FS_SECTOR_SIZE ? FS_SECTOR_SIZE : remaining;
        if (disk_read_sector(FS_LBA_START + s, sector) < 0) return -1;
        if (copy_len) memcpy(raw + byte_offset, sector, copy_len);
    }

    if (fs_disk_image.magic != FS_MAGIC || fs_disk_image.version != 1) {
        fs_init();
        return fs_save();
    }

    memcpy(fs, fs_disk_image.files, sizeof(fs));
    for (int i = 0; i < MAX_FILES; i++) {
        if (fs[i].used) {
            fs[i].name[NAME_LEN - 1] = 0;
            fs[i].data[FILE_SIZE - 1] = 0;
        }
    }
    return 0;
}

int fs_write(const char* name, const char* data) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (fs[i].used) {
            int match = 1;
            for (int j = 0; name[j] || fs[i].name[j]; j++) {
                if (name[j] != fs[i].name[j]) {
                    match = 0;
                    break;
                }
            }
            if (match) {
                int k = 0;
                while (data[k] && k < FILE_SIZE - 1) {
                    fs[i].data[k] = data[k];
                    k++;
                }
                fs[i].data[k] = 0;
                return fs_save();
            }
        }
    }

    for (int i = 0; i < MAX_FILES; i++) {
        if (!fs[i].used) {
            int j = 0;
            while (name[j] && j < NAME_LEN - 1) {
                fs[i].name[j] = name[j];
                j++;
            }
            fs[i].name[j] = 0;
            fs[i].used = 1;

            int k = 0;
            while (data[k] && k < FILE_SIZE - 1) {
                fs[i].data[k] = data[k];
                k++;
            }
            fs[i].data[k] = 0;
            return fs_save();
        }
    }
    return -1;
}

int fs_create(const char* name) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (!fs[i].used) {
            int j = 0;
            while (name[j] && j < NAME_LEN - 1) {
                fs[i].name[j] = name[j];
                j++;
            }
            fs[i].name[j] = 0;
            fs[i].used = 1;
            fs[i].data[0] = 0;
            return fs_save();
        }
    }
    return -1;
}

void fs_list() {
    for (int i = 0; i < MAX_FILES; i++) {
        if (fs[i].used) {
            print(fs[i].name);
            print("\n");
        }
    }
}

#endif