bits 32
global start

extern exit, printf
import exit msvcrt.dll
import printf msvcrt.dll
extern sum

segment data use32 class=data
    a dd 10
    b dd 20
    format db "%d + %d = %d", 13, 10, 0

segment code use32 class=code
start:
    ;sum(a,b)
    ;return value in eax = a+b
    ;free in procedure
    push dword [b]
    push dword [a]
    call sum
    add esp, 4*2

    ;printf("%d + %d = %d", a, b, eax)
    push dword eax
    push dword [b]
    push dword [a]
    push dword format
    call [printf]
    add esp, 4*4

	push dword 0 
	call [exit] 