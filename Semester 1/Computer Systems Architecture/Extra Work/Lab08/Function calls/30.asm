; Read numbers (in base 10) in a loop until the digit '0' is read from the keyboard. 
; Determine and display the smallest number from those that have been read.

bits 32
global start

import scanf msvcrt.dll
import exit msvcrt.dll
import printf msvcrt.dll

extern scanf, exit, printf

segment data use32 class=data
    number dd 0
    smallest dd 07FFFFFFFh; a large number in unsigned
    read_format db "%d", 0
    print_format db "Smallest number read is %d", 0

segment code use32 class=code
    start:

        continue_read:
            push number
            push read_format
            call [scanf]
            add esp, 4*2

            ; when 0 is read, the reading of numbers is finished
            cmp dword [number], 0
            je print_smallest

            mov eax, [number]; eax = number
            cmp eax, [smallest]; if eax < smallest => smallest = eax
            jge no_change ; signed comparison

            ; if eax is bigger, change the value of smallest
            mov [smallest], eax

            no_change:
                jmp continue_read
            
        print_smallest:
            push dword [smallest]
            push print_format
            call [printf]
            add esp, 4*2


        push dword 0
        call [exit]
