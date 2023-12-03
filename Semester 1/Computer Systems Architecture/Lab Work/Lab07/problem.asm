; n citit %i sau %d (-8)
; afisare: nr uns: %u, nr hexa: %X/%x

bits 32
global start
extern exit, printf, scanf
import exit msvcrt.dll
import printf msvcrt.dll
import scanf msvcrt.dll

segment data use32 class=data
	n dd 0
	mes db "Unsigned = %u", 10, 13, "Hexa = %X", 0
	format_unsigned db "%u"
	
segment code use32 class=code
start:
	push dword n
	push dword format_unsigned
	call [scanf]
	add ESP, 4*2
	
	push dword[n]
	push dword[n]
	push dword mes
	call [printf]
	add ESP, 4*3
	
	push dword 0
	call [exit]