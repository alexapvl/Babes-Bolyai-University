; Read a hexadecimal number with 2 digits from the keyboard. 
; Display the number in base 10, in both interpretations: 
; as an unsigned number and as an signed number (on 8 bits).

bits 32
global start

import exit msvcrt.dll
import scanf msvcrt.dll
import printf msvcrt.dll

extern exit, scanf, printf

segment data use32 class=data
	number dd 0
	read_format db "%x", 0
	print_format db "Unsigned = %u; Signed = %i", 0 ; for each %.. we need a value pushed onto the stack => we will need two values

segment code use32 class=code
start:

	push number
	push read_format
	call [scanf]
	add esp, 4*2

	; convert the 2 hexadigits (1 byte) read to a dword in eax which will be pushed as a parameter into printf function
	mov al, [number]
	cbw
	cwde

	push eax ; signed conversion to doubleword because two hexadecimal will only occupy a byte and the rest will be zeros
	push dword [number] ; number but unchanged because we need the unsigned version of it so we do not care if the rest of the bytes are all zeros
	push print_format
	call [printf]
	add esp, 4*2

	push dword 0 
	call [exit] 
