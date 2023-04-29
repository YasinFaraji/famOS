section .asm

global idt_load

idt_load:
    push ebp
    mov ebp, esp
    
    mov ebx, [ebp+8]    ; ebp + 8 -> this point to first arguments of function.
    lidt [ebx]
    pop ebp
    ret