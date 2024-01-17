; Read numbers (in base 10) in a loop until the digit '0' is read from the keyboard. 
; Determine and display the greatest number from those that have been read.

bits 32
global start

import scanf msvcrt.dll
import exit msvcrt.dll
import printf msvcrt.dll

extern scanf, exit, printf

segment data use32 class=data
    number dd 0
    greatest dd 080000000h; small number in signed
    read_format db "%d", 0
    print_format db "Greatest number read is %i", 0

segment code use32 class=code
    start:

        continue_read:
            push number
            push read_format
            call [scanf]
            add esp, 4*2

            ; when 0 is read, the reading of numbers is finished
            cmp dword [number], 0
            je print_greatest

            mov eax, [number]; eax = number
            cmp eax, [greatest]; if eax < smallest => smallest = eax
            jle no_change

            ; if eax is bigger, change the value of smallest
            mov [greatest], eax

            no_change:
                jmp continue_read
            
        print_greatest:
            push dword [greatest]
            push print_format
            call [printf]
            add esp, 4*2


        push dword 0
        call [exit]
