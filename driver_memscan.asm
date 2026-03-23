[org 0x9000]
bits 16

ARG_TABLE_ADDR  equ 0x7000 ; ignore the peculiar "can i see your dih" comment. i was feeling a bit energetic.
ARG_SIZE        equ 32

driver_main:
    mov bx, ARG_TABLE_ADDR
    mov ax, [bx]
    mov [arg_count], ax

    cmp ax, 0
    je do_scan

    add bx, 2
    mov si, bx
    mov di, scan_start_str
    mov cx, ARG_SIZE
    rep movsb

    cmp ax, 1
    je do_scan

    mov si, bx
    add si, ARG_SIZE
    mov di, scan_end_str
    mov cx, ARG_SIZE
    rep movsb

do_scan:
    mov si, msg_scanning
    call print_string

    mov ax, [scan_start_str]
    cmp ax, 0
    je .use_defaults
    call parse_hex
    mov [scan_start], ax

    mov si, scan_end_str
    mov ax, [si]
    cmp ax, 0
    je .use_defaults
    call parse_hex
    mov [scan_end], ax
    jmp .begin_scan

.use_defaults:
    mov word [scan_start], 0x0000
    mov word [scan_end],   0xFFFF

.begin_scan: ; can i see your dih (your dih)? and can you make it super veiny (veinyyyy)? i said im about to pmo (pmooo), i dont know if youre gonna see us agaaainn can i see your dih (can iiii) and can you make it super veiny (veinyy) i said im about to pmo (pmooo) im not sure if you will see us againnn, Ts pmo ts pmo ts pmo ts pmo ts pmo ts pmo icl tbh sybau twin
    mov si, msg_range
    call print_string
    mov ax, [scan_start]
    call print_hex
    mov si, msg_dash
    call print_string
    mov ax, [scan_end]
    call print_hex
    call print_newline

    mov bx, [scan_start]
    mov cx, 0
.scan_loop:
    cmp bx, [scan_end]
    ja .scan_done
    mov al, [bx]
    cmp al, 0
    je .next
    inc cx
.next:
    inc bx
    jmp .scan_loop

.scan_done:
    mov si, msg_nonzero
    call print_string
    mov ax, cx
    call print_hex
    call print_newline
    ret

parse_hex:
    mov ax, 0
    mov cx, 0
.next_char:
    mov bl, [si]
    cmp bl, 0
    je .done
    cmp bl, '0'
    jl .done
    cmp bl, '9'
    jle .is_digit
    cmp bl, 'A'
    jl .done
    cmp bl, 'F'
    jle .is_upper
    cmp bl, 'a'
    jl .done
    cmp bl, 'f'
    jg .done
    sub bl, 32
.is_upper:
    sub bl, 7
.is_digit:
    sub bl, '0'
    shl ax, 4
    or al, bl
    inc si
    jmp .next_char
.done:
    ret

print_hex:
    mov cx, 4
.loop:
    rol ax, 4
    mov bx, ax
    and bx, 0x000F
    mov bl, [hex_chars + bx]
    mov ah, 0x0E
    int 0x10
    loop .loop
    ret

print_string:
    lodsb
    cmp al, 0
    je .done
    mov ah, 0x0E
    int 0x10
    jmp print_string
.done:
    ret

print_newline:
    mov ah, 0x0E
    mov al, 0x0D
    int 0x10
    mov al, 0x0A
    int 0x10
    ret

msg_scanning    db "memscan: scanning memory...", 13, 10, 0
msg_range       db "memscan: range 0x", 0
msg_dash        db "-0x", 0
msg_nonzero     db "memscan: non-zero bytes: 0x", 0
hex_chars       db "0123456789ABCDEF"

arg_count       dw 0
scan_start      dw 0x0000
scan_end        dw 0xFFFF
scan_start_str  times 32 db 0
scan_end_str    times 32 db 0