; Read two numbers a and b (in base 10) from the keyboard and determine the order 
; relation between them (either a < b, or a = b, or a > b). 
; Display the result in the following format: "<a> < <b>, <a> = <b> or <a> > <b>".

bits 32
global start

import exit msvcrt.dll
import scanf msvcrt.dll
import printf msvcrt.dll

extern exit, scanf, printf

segment data use32 class=data
    a dd 0
    b dd 0
    scan_format db "%d", 0
    a_less_than_b db "%d < %d", 0
    a_equal_b db "%d = %d", 0
    a_greater_than_b db "%d > %d", 0

segment code use32 class=code
start:
    push a
    push scan_format
    call [scanf]

    push b
    push scan_format
    call [scanf]

    mov eax, [a]

    cmp eax, [b]
    jl less
    je equal
    jg greater

    less:
        push dword [b]
        push dword [a]
        push a_less_than_b
        call [printf]
        add esp, 4*3
        jmp finish
    
    equal:
        push dword [b]
        push dword [a]
        push a_equal_b
        call [printf]
        add esp, 4*3
        jmp finish
    
    greater:
        push dword [b]
        push dword [a]
        push a_greater_than_b
        call [printf]
        add esp, 4*3
        jmp finish

    finish:

	push dword 0 
	call [exit] 
