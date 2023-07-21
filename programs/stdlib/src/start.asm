[BITS 32]

global _start
extern c_start
extern famos_exit

section .asm

_start:
    call c_start
    call famos_exit
    ret