[org 0x8000]
bits 16

DRIVER_LOAD_ADDR    equ 0x9000
ARG_TABLE_ADDR      equ 0x7000
ARG_MAX             equ 10
ARG_SIZE            equ 32

%macro print 1 
    jmp %%after ; if macro's didnt exist, i would personally visit the assembly inventor´s residence.
    %%str db %1, 0
%%after:
    mov si, %%str
    call print_string ; the print_string function is still pretty outdated but im wayyy too lazy to change it
%endmacro

%macro newlineprint 1
    jmp %%after
    %%str db %1, 0
%%after:
    mov si, %%str
    call print_string
    call print_newline
%endmacro

%macro choice 2
    mov ah, 0
    int 0x16
    cmp al, %1
    je %2
%endmacro

%macro choice_loop 3
%%wait:
    mov ah, 0
    int 0x16
    cmp al, %1
    jne %%wait
    je %2
%endmacro

; okay so this load driver macro might just be the most autistic macro that has ever been made.

%macro load_driver 1 
    jmp %%after
    %%fname db %1, 0
%%after:
    mov si, %%fname
    call clear_arg_table
    call do_load_driver
%endmacro

kernel_main: ; AFTER THE TEST DRIVER AND LOADING THE BOOTLOADER IT SHOULD DO BLOODY THIS
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    sti

    newlineprint "-----------------------------------------------------------------------"
    newlineprint "                   "
    newlineprint "                   "
    newlineprint "  /=====-----=====\"
    newlineprint "  |!! MICRO OS !!-|"
    newlineprint "  |ASSEMBLY KERNEL|"
    newlineprint "  |Codename:Cwind |"
    newlineprint "  \=====-----=====/"
    newlineprint " "
    newlineprint " "
    newlineprint "  USE THE COMMAND: 'continue' TO GET INTO THE BETTER KERNEL."
    newlineprint "  I PUT MORE UPDATES AND EFFORT INTO THE BETTER KERNEL THAN THIS ONE!"
    newlineprint " "
    newlineprint " "
    newlineprint "-----------------------------------------------------------------------"

    call checkifstartatc

    mov si, welcome1
    call print_string
    jmp restart

checkifstartatc:
    cmp byte [StartAtC], 1
    je cmd_continue
    jne .done

.done:
    ret


cmd_continue:
    mov si, msg_continue
    call print_string
    call print_newline

    cli

    ; build GDT at 0x7E00
    mov di, 0x7E00
    mov dword [di],    0
    mov dword [di+4],  0
    mov word  [di+8],  0xFFFF
    mov word  [di+10], 0x0000
    mov byte  [di+12], 0x00
    mov byte  [di+13], 0x9A
    mov byte  [di+14], 0xCF
    mov byte  [di+15], 0x00
    mov word  [di+16], 0xFFFF
    mov word  [di+18], 0x0000
    mov byte  [di+20], 0x00
    mov byte  [di+21], 0x92
    mov byte  [di+22], 0xCF
    mov byte  [di+23], 0x00
    mov word  [di+24], 23
    mov dword [di+26], 0x7E00

    lgdt [0x7E18]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp 0x08:pm_enter

msg_continue db "Switching to advanced mode...", 0

bits 32
pm_enter:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    call 0x50000

    cli
    hlt

gdt_start:
    dd 0
    dd 0

    
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10011010b
    db 11001111b
    db 0x00

    ; small pp energy type of data buffers, the manual absolutely carried me through this one since i cant memorize whatever this is
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

bits 32
protected_mode:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    call 0x50000        ; C kernel lives here now yipeeee

    cli
    hlt

bits 16

restart:
    mov di, input_buffer
    mov cx, 128
    xor al, al
    rep stosb
    mov di, cmd_buffer
    mov cx, 64
    rep stosb
    mov di, arg_buffer
    mov cx, 64
    rep stosb

    mov si, prompt
    call print_string
    call input_line
    call parse_args

    mov si, cmd_buffer
    mov di, command_table
    jmp next_cmd

next_cmd:
    cmp byte [di], 0
    je unknown_cmd
    push si
    push di
    inc di
    call strcmp
    pop di
    pop si
    cmp ax, 1
    je run_cmd
    jmp skip_cmd

skip_cmd:
    inc di
skip_loop:
    cmp byte [di], 0
    je skip_end
    inc di
    jmp skip_loop
skip_end:
    inc di
    add di, 2
    mov si, cmd_buffer
    jmp next_cmd

run_cmd:
    inc di
find_end:
    cmp byte [di], 0
    je addr
    inc di
    jmp find_end
addr:
    inc di
    mov bx, [di]
    call bx
    jmp restart

unknown_cmd:
    mov si, msg_unknown
    call print_string
    jmp restart

parse_args:
    mov si, input_buffer
    mov di, cmd_buffer
.copy_cmd:
    mov al, [si]
    cmp al, 0
    je .done
    cmp al, ' '
    je .found_space
    mov [di], al
    inc si
    inc di
    jmp .copy_cmd
.found_space:
    mov byte [di], 0
    inc si
    mov di, arg_buffer
.copy_arg:
    mov al, [si]
    mov [di], al
    cmp al, 0
    je .done
    inc si
    inc di
    jmp .copy_arg
.done:
    mov byte [di], 0
    ret

cmd_loaddriver:
    mov si, arg_buffer
    mov di, drv_filename
    mov cx, 0

    cmp byte [si], '"'
    jne .no_quote
    inc si
.no_quote:
.read_fname: 
    mov al, [si]
    cmp al, 0
    je .fname_done
    cmp al, '"'
    je .fname_quoted_end
    cmp al, ' '
    je .fname_done
    mov [di], al
    inc si
    inc di
    inc cx
    cmp cx, 63
    jl .read_fname
.fname_quoted_end:
    inc si
.fname_done:
    mov byte [di], 0

    cmp byte [si], ' '
    jne .no_space_after
    inc si
.no_space_after:
    call clear_arg_table
    call parse_driver_args
    mov si, drv_filename
    call do_load_driver
    ret

parse_driver_args:
    mov bx, ARG_TABLE_ADDR
    mov word [bx], 0
    add bx, 2
    mov cx, 0
.next_arg:
    cmp cx, ARG_MAX
    jge .done
    cmp byte [si], 0
    je .done
    mov di, bx
    mov dx, 0
.copy_char:
    mov al, [si]
    cmp al, 0
    je .arg_end
    cmp al, ' '
    je .arg_end
    mov [di], al
    inc si
    inc di
    inc dx
    cmp dx, ARG_SIZE - 1
    jl .copy_char
.arg_end:
    mov byte [di], 0
.skip_spaces:
    cmp byte [si], ' '
    jne .no_space
    inc si
    jmp .skip_spaces
.no_space: ; the most autistic part of getting input of assembly, the manual claimed that this is the only way
    inc cx
    mov [ARG_TABLE_ADDR], cx
    add bx, ARG_SIZE
    jmp .next_arg
.done:
    ret

clear_arg_table:
    mov di, ARG_TABLE_ADDR
    mov cx, 2 + (ARG_MAX * ARG_SIZE)
    xor al, al
    rep stosb
    ret

commitkys: ; 
    ; gaslight the CPU into shutting the ever living fuck up. This is one of the safest ways to turn it off, But fuck the CPU like shut the fuck up for once
    mov ax, 0x5301      
    xor bx, bx          
    int 0x15
    jc .fail            ; carry set becomes a big stinky disgusting error

    ; state 0x0003 just means: fuck it, Go kill yourself. CPU. turn yourself off. make it painfull.
    mov ax, 0x5307      
    mov bx, 0x0001      
    mov cx, 0x0003      
    int 0x15
    jc .fail
    .fail:
    ret
    jmp $ ; nice


do_load_driver:
    mov di, driver_registry
.search:
    cmp byte [di], 0
    je .not_found
    push si
    push di
    call strcmp
    pop di
    pop si
    cmp ax, 1
    je .found
.skip_entry:
    inc di
    cmp byte [di], 0
    jne .skip_entry
    inc di
    inc di
    jmp .search
.found:
.find_sector:
    .find_sector:
    inc di
    cmp byte [di-1],0
    jne .find_sector

    mov cl,[di]      
    inc di
    mov al,[di]     

    mov ah,0x02    
    mov ch,0        
    mov dh,0         
    mov dl,0x00     
    mov bx,DRIVER_LOAD_ADDR

    int 0x13        

    jc .disk_error  

    mov bx,ARG_TABLE_ADDR
    mov ax,DRIVER_LOAD_ADDR
    call ax       

.not_found:
    mov si, msg_drv_notfound
    call print_string
    ret
.disk_error:
    mov si, msg_drv_diskerr
    call print_string
    ret

print_hello:
    mov si, msg_hello
    call print_string
    ret

cmd_echo:
    mov si, arg_buffer
    call print_string
    call print_newline
    ret

clear_screen: ; DUDE WORK WHY WHY WHY WHY WHY
    mov ax, 0x0600
    mov bh, 0x07
    mov cx, 0x0000
    mov dx, 0x184F
    int 0x10
    mov ah, 0x02
    mov bh, 0x00
    xor dx, dx
    int 0x10
    ret

appsmenu:
    ret

strcmp:
    mov ax, 0
cmp_loop:
    mov al, [si]
    mov bl, [di]
    cmp al, bl
    jne cmp_no
    cmp al, 0
    je cmp_yes
    inc si
    inc di
    jmp cmp_loop
cmp_yes:
    mov ax, 1
    ret
cmp_no:
    xor ax, ax
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

input_line:
    mov di, input_buffer
il1:
    mov ah, 0
    int 0x16
    cmp al, 0x0D
    je il_done
    cmp al, 0x08
    je il_back
    mov ah, 0x0E
    int 0x10
    stosb
    jmp il1
il_back:
    cmp di, input_buffer
    je il1
    dec di
    mov ah, 0x0E
    mov al, 0x08
    int 0x10
    mov al, ' '
    int 0x10
    mov al, 0x08
    int 0x10
    jmp il1
il_done:
    mov byte [di], 0
    call print_newline
    ret

print_single_char:
    mov ah, 0x0E
    int 0x10
    ret

print_char_range:
    push ax
    push bx

    mov bl, al      
    mov bh, ah      

.next_char:
    mov al, bl
    call print_single_char
    inc bl
    cmp bl, bh
    jbe .next_char

    pop bx
    pop ax
    ret


functestuncfunc_everycharascii:
    newlineprint "Test chars: "
    mov al, 0x20  
    mov ah, 0x7E    ; last ascii which is "~" and its super skibidi 67 sigma
    call print_char_range
    call print_newline
    ret


; the data section [[the old stuff, before i realized i could make a newlineprint macro]]

welcome1         db "  welcome to MicroOS", 13, 10, 0
prompt           db "> ", 0
msg_hello        db "Hello from MicroOS!", 13, 10, 0
msg_unknown      db "Unknown command", 13, 10, 0
msg_drv_notfound db "loaddriver: unknown driver", 13, 10, 0
msg_drv_diskerr  db "loaddriver: disk error", 13, 10, 0

StartAtC db 1 ; To any regular user like people that cloned or pulled this from github, It's reccomended you keep this at 1. The assembly kernel lacks usefull features.

input_buffer    times 128 db 0  
cmd_buffer      times 64  db 0
arg_buffer      times 64  db 0
drv_filename    times 64  db 0

driver_registry: 

    db "driver_memscan.bin",0
    db "driver_test.bin", 0
    db 10        
    db 4         

    db 0

firsttestdriver:
    jmp 0x9500

    .done:
        ret

command_table:
    db 5, "tinyhello", 0
    dw print_hello
    db 5, "clear", 0
    dw clear_screen
    db 4, "apps", 0
    dw appsmenu
    db 4, "echo", 0
    dw cmd_echo
    db 10, "loaddriver", 0
    dw cmd_loaddriver
    db 4, "shutdown", 0
    dw commitkys
    db 4, "Microos_Driver_Test", 0 
    dw firsttestdriver
    db 4, "chartest", 0
    dw functestuncfunc_everycharascii
    db 8, "continue", 0
    dw cmd_continue
    db 0