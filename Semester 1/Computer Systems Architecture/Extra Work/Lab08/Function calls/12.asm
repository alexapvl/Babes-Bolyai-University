; A negative number a (a: dword) is given. 
; Display the value of that number in base 10 and in the base 16 in the following format: 
; "a = <base_10> (base 10), a = <base_16> (base 16)"
bits 32
global start

extern exit, printf, scanf
import exit msvcrt.dll
import scanf msvcrt.dll
import printf msvcrt.dll

segment data use32 class=data
    a dd -12
    pf db "a = %i (base 10), a = %X (base 16)", 0

segment code use32 class=code
start:
    push dword [a]; push once for %X
    push dword [a]; push second time for %i
    push pf
    call [printf]
    add esp, 4*3

	push dword 0 
	call [exit] 