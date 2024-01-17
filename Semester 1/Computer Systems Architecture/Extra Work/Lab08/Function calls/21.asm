; Read two words a and b in base 10 from the keyboard. 
; Determine the doubleword c such that the low word of c is given by 
; the sum of the a and b and the high word of c is given by the difference between a and b. 
; Display the result in base 16 
; Example:
; a = 574, b = 136
; c = 01B602C6h

bits 32
global start

import exit msvcrt.dll
import scanf msvcrt.dll
import printf msvcrt.dll

extern exit, scanf, printf

segment data use32 class=data
    a dw 0
    b dw 0
    read_format db "%d", 0
    c dd 0
    print_format db "c = %X"

segment code use32 class=code
start:
    ; read a
    push a
    push read_format
    call [scanf]
    add esp, 4*2

    ; read b
    push b
    push read_format
    call [scanf]
    add esp, 4*2

    mov ax, [a]
    add ax, [b]
    mov [c], ax

    mov ax, [a]
    sub ax, [b]
    mov [c+2], ax

    push dword [c]
    push print_format
    call [printf]
    add esp, 4*2

	push dword 0 
	call [exit] 
