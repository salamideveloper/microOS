; entry.asm
; the smallest pieces of code are the most important
; okay so here is a cool story,
; i was in class, then i breathed, i was still in class.
[bits 32]
[extern kernel_main]

global _start
_start:
    call kernel_main
    cli
    hlt