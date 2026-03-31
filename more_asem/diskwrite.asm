bits 32

global _disk_read_lba
global _disk_write_lba

; ATA ports
PORT_DATA   equ 0x1F0
PORT_SECCNT equ 0x1F2
PORT_LBA0   equ 0x1F3
PORT_LBA1   equ 0x1F4
PORT_LBA2   equ 0x1F5
PORT_DRIVE  equ 0x1F6
PORT_CMD    equ 0x1F7
PORT_STATUS equ 0x1F7

section .text

; -----------------------------------------
; void _disk_read_lba(int lba, int sector_count, void* buffer)
; eax = lba
; ecx = sector count
; edi = buffer
; -----------------------------------------
_disk_read_lba:
    pusha

    mov dx, PORT_DRIVE
    mov al, 0xE0
    or al, ah
    out dx, al

    mov dx, PORT_SECCNT
    mov al, cl
    out dx, al

    mov dx, PORT_LBA0
    mov al, al
    out dx, al

    mov dx, PORT_LBA1
    shr eax, 8
    out dx, al

    mov dx, PORT_LBA2
    shr eax, 16
    out dx, al

    mov dx, PORT_CMD
    mov al, 0x20
    out dx, al

.wait:
    in al, dx
    test al, 8
    jz .wait

    mov ecx, 256
    mov dx, PORT_DATA

.read:
    in ax, dx
    stosw
    loop .read

    popa
    ret


; -----------------------------------------
; void _disk_write_lba(int lba, int sector_count, void* buffer)
; eax = lba
; ecx = sector count
; esi = buffer
; -----------------------------------------
_disk_write_lba:
    pusha

    mov dx, PORT_DRIVE
    mov al, 0xE0
    or al, ah
    out dx, al

    mov dx, PORT_SECCNT
    mov al, cl
    out dx, al

    mov dx, PORT_LBA0
    mov al, al
    out dx, al

    mov dx, PORT_LBA1
    shr eax, 8
    out dx, al

    mov dx, PORT_LBA2
    shr eax, 16
    out dx, al

    mov dx, PORT_CMD
    mov al, 0x30
    out dx, al

.wait:
    in al, dx
    test al, 8
    jz .wait

    mov ecx, 256
    mov dx, PORT_DATA

.write:
    mov ax, [esi]
    out dx, ax
    add esi, 2
    loop .write

    popa
    ret