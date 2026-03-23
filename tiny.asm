[org 0x7C00]
bits 16

; ts is the bootloader for microOS. If you read it carefully. You can strongly observe how autistic this looks.

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    sti

    mov [boot_drive], dl

    mov si, msg1
    call print_string
    mov si, msg2
    call print_string
    mov si, msg3
    call print_string

    ; load assembly kernel at 0x8000
    mov ax, 0x0000
    mov es, ax
    mov bx, 0x8000
    mov ah, 0x02
    mov al, 32
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, [boot_drive]
    int 0x13
    jc disk_error

    mov si, msg4
    call print_string

    ; load C kernel at 0x50000
    mov ax, 0x5000
    mov es, ax
    mov bx, 0x0000
    mov ah, 0x02
    mov al, 64
    mov ch, 0
    mov cl, 41
    mov dh, 0
    mov dl, [boot_drive]
    int 0x13
    jc disk_error

    mov si, msg5
    call print_string

    jmp 0x0000:0x8000

disk_error:
    mov si, msg_err
    call print_string
    hlt

print_string:
    lodsb
    cmp al, 0
    je .done
    mov ah, 0x0E
    int 0x10
    jmp print_string
.done:
    ret

boot_drive  db 0
msg1        db "[|] MicroOS Bootloader", 13, 10, 0
msg2        db "[*] Loading assembly kernel", 13, 10, 0
msg3        db "[*] Kernel is at 0x8000", 13, 10, 0
msg4        db "[*] Loading C kernel", 13, 10, 0
msg5        db "[*] C kernel is at 0x50000", 13, 10, 0
msg_err     db "[!] Disk read error!", 13, 10, 0

times 510-($-$$) db 0
dw 0xAA55