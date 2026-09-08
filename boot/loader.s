global loader
extern kernel_main              ; Tell Assembly that kernel_main lives in another file (kernel.c)

MAGIC_NUMBER equ 0x1BADB002
FLAGS        equ 0x0
CHECKSUM     equ -MAGIC_NUMBER

section .text
align 4
    dd MAGIC_NUMBER
    dd FLAGS
    dd CHECKSUM

loader:
    mov esp, kernel_stack + 4096 ; Set up a stack pointer for C to use variables
    call kernel_main             ; Call your C function!
.loop:
    jmp .loop                    ; Safety backup loop

section .bss
align 4
kernel_stack:
    resb 4096                    ; Allocate 4 Kilobytes of memory space for the C stack
