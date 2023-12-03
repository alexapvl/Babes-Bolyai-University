; Read a number in base 10 from the keyboard and display the value of that number in base 16 
; Example: Read: 28; display: 1C
bits 32
global start

extern exit, scanf, printf
import exit msvcrt.dll
import scanf msvcrt.dll
import printf msvcrt.dll

segment data use32 class=data
    number dd 0
    read_format dd "%d", 0
    string1 db "Number in base 10: ", 0
    string2 db "Number in base 16: %X", 0

segment code use32 class=code
start:
    push string1
    call [printf]
    add esp, 4

    ;read number for user
    push number ; pushed an address, so we pushed a dword to the stack
    push read_format
    call [scanf]
    add esp, 4*2

    push dword [number] ; pushed the value of the number onto the stack under the form of a doubleword -> convention
    push string2
    call [printf]
    add esp, 4*2

	push dword 0 
	call [exit] 