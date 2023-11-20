; Two natural numbers a and b (a: dword, b: dword, defined in the data segment) are given.
; Calculate a/b and display the quotient in the following format: "<a>/<b> = <quotient>".
; Example: for a = 200, b = 5 it will display: "200/5 = 40".
; The values will be displayed in decimal format (base 10) with sign.

bits 32
global start

extern exit, printf
import exit msvcrt.dll
import printf msvcrt.dll

segment data use32 class=data
	a dd 200
	b dd 5
	quotient dd 0
	msg db "%i/%i = %i", 0

segment code use32 class=code
start:
	mov eax, [a]
	cdq; eax -> edx:eax
	idiv dword [b]; quotient will be in EAX
	mov [quotient], eax
	push dword [quotient]
	push dword [b]
	push dword [a]
	push msg
	call [printf]
	add esp, 4*4

	push dword 0
	call [exit]
