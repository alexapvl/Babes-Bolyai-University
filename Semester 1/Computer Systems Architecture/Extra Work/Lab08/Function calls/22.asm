; Two numbers a and b are given. Compute the expression value: (a+b)*k,
;  where k is a constant value defined in data segment. Display the expression value (in base 10).

bits 32
global start

import exit msvcrt.dll
import printf msvcrt.dll

extern exit, printf

segment data use32 class=data
    a dw 10
    b dw 10
    k dw -10
    result dd 0
    print_format db "Result of (a+b)*k is %i", 0

segment code use32 class=code
start:
    mov ax, [a]; AX = a
    add ax, [b]; AX = a+b

    imul word [k]; DX:AX = (a+b)*k
    mov [result], ax
    mov [result+2], dx; result = DX:AX

    push dword [result]
    push print_format
    call [printf]
    add esp, 4*2

	push dword 0 
	call [exit] 
