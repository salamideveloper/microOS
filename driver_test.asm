[org 0x9500]
bits 16

%macro newlineprint 1
    jmp %%after
    %%str db %1, 0
%%after:
    mov si, %%str
    call print_string
    call print_newline
%endmacro


start:
    newlineprint "Test driver"
    ret ; THIS ONE LITTLE FUCKER FIXED THE ENTIRE DRIVER. I FORGOT TO RETURN. WHAT THE FUCK.
    
.done: 
    newlineprint "2"
    jmp 0x8000 


print_newline:
    mov ah, 0x0E
    mov al, 0x0D
    int 0x10
    mov al, 0x0A
    int 0x10
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

