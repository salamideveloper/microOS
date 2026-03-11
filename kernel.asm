[org 0x8000]
bits 16

DRIVER_LOAD_ADDR    equ 0x9000
ARG_TABLE_ADDR      equ 0x7000
ARG_MAX             equ 10
ARG_SIZE            equ 32

%macro print 1 
    jmp %%after ; if macro's didnt exist, i would personally visit the assembly woman's residence.
    %%str db %1, 0
%%after:
    mov si, %%str
    call print_string ; i genuinely dont know what call fully does. i just saw it in a output manual.
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

kernel_main:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    sti

    newlineprint "                   "
    newlineprint "                   "
    newlineprint "  /=====-----=====\"
    newlineprint "  |!! MICRO OS !!-|"
    newlineprint "  \=====-----=====/"
    newlineprint "                   "
    newlineprint "                   "

    mov si, welcome1
    call print_string
    jmp restart

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
.no_space:
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

commitkys: ; what a name, i had to think of something creative.
    ; the most annoying but least shit way to shut a CPU down. straight autism.
    mov ax, 0x5301      ; 0x5301 is a specific autistic ass port that connects to the APM registers inside x86 cpu's. tragic
    xor bx, bx          ; APM bios specifically 
    int 0x15
    jc .fail            ; carry set becomes a big stinky disgusting error

    ; state 0x0003 just means: fuck it, KYS. CPU. turn yourself off. make it painfull.
    mov ax, 0x5307      ; the power state register in APM. still tragically autistic
    mov bx, 0x0001      ; all devices, YES. EVERYTHING. kill everything and then commit KYS yourself
    mov cx, 0x0003      ; KYS
    int 0x15
    jc .fail
    .fail:
    ret
    ; if it worked, theres no need to return. so jmp $ so just makes the CPU not commit autistic crimes.
    jmp $


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
    mov si, apps_str1
    call print_string
    mov si, apps_str2
    call print_string
    mov si, apps_str3
    call print_string
    mov si, apps_str4
    call print_string
    mov si, apps_str5
    call print_string
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

; the data section 

welcome1         db "  welcome to MicroOS", 13, 10, 0
apps_str1        db "  commands available:", 13, 10, 0
apps_str2        db "  hello      - say hello", 13, 10, 0
apps_str3        db "  clear      - clear screen", 13, 10, 0
apps_str4        db "  echo       - echo argument", 13, 10, 0
apps_str5        db "  loaddriver - load a driver", 13, 10, 0
prompt           db "> ", 0
msg_hello        db "Hello from MicroOS!", 13, 10, 0
msg_unknown      db "Unknown command", 13, 10, 0
msg_drv_notfound db "loaddriver: unknown driver", 13, 10, 0
msg_drv_diskerr  db "loaddriver: disk error", 13, 10, 0

input_buffer    times 128 db 0
cmd_buffer      times 64  db 0
arg_buffer      times 64  db 0
drv_filename    times 64  db 0

driver_registry:

    db "driver_memscan.bin",0
    db 10        
    db 4         

    db 0


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
    db 0
