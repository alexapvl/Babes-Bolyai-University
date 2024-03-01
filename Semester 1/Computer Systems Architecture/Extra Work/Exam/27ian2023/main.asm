bits 32
global start
extern exit, calculate, printf, scanf

import exit msvcrt.dll
import printf msvcrt.dll
import scanf msvcrt.dll

segment data use32 class=data
    S dq 0x1122334455667788, 0x99AABBCCDDEEFF11, 0xFF00000000000055, 0xFFFEFFFFFFFFEFFF, 0x450000AB000000AB, 0x11113733555577AB
    len equ $-S; length of S
    counter db 0; to count how many bytes will be in the new string
    N dd -1; read from keyboard
    print_string db 0, 0
    new_string resb 100
    scan_format db '%d', 0
    print_format db '%X', 0

    segment code use32 class=code
        start:
            push dword N
            push dword scan_format
            call [scanf]
            add esp, 4*2

            cmp byte [N], 1
            jl finish
            cmp byte [N], 6
            jg finish

            cmp byte [counter], len
            jae finish_string

            mov esi, S
            mov edi, new_string
            cld

            form_string:
                movsb; leave edi as it is
                dec esi; reverse the incrementation to do out own, by N
                add esi, [N]
                inc byte [counter]
                cmp byte [counter], len
                jae finish_string
                jmp form_string
            
            finish_string:
                xor ecx, ecx; force the value of ecx to 0
                mov cl, [counter]
                times 2 dec cl; max index to which edi ewill go(index len - 2)
                jecxz print

                outer_loop:
                    mov edi, 0; loop counter
                    mov edx, 0; flag to mark is a swap occured in the inner loop

                inner_loop:
                    mov al, [new_string + edi]
                    mov bl, [new_string + edi + 1]
                    push ecx
                    call calculate; will put in AH <- 1 if there will be a swap needed, 0 otherwise
                    pop ecx

                    cmp ah, 1
                    jne no_swap
                    ;make swap
                    mov [new_string + edi], bl
                    mov [new_string + edi + 1], al
                    mov edx, 1; mark the swap occured

                no_swap:
                    inc edi; update inner loop counter
                    cmp edi, ecx
                    jl inner_loop
                ;check to se if any changes were made in the inner loop
                
                cmp edx, 1
                je outer_loop; swaps were made, string potentially not sorted yet

            print: ; take every byte of the string and print it
                mov edi, 0; index for string
                ;ecx is still the same value as in the beginning
                jecxz finish

                continue_print:
                    ;put the next byte of the string in a separate string and print it
                    mov al, [new_string + edi]
                    mov [print_string], al
                    push print_string
                    push print_format
                    call [printf]
                    add esp, 4*2; clear stack
                    inc edi
                    cmp edi, ecx
                    jl continue_print
            
            finish:
                push dword 0
                call [exit]
