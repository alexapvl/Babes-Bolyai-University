; A file name and a text (which can contain any type of character) are given in data segment. 
; Calculate the sum of digits in the text. 
; Create a file with the given name and write the result to file.


bits 32
global start

extern exit, fopen, fclose, fprintf
import exit msvcrt.dll
import fopen msvcrt.dll
import fclose msvcrt.dll
import fprintf msvcrt.dll

segment data use32 class=data
    output_file db "output.txt", 0
    write db "w", 0
    file_desc dd -1
    print_text db "The sum of the digits in the text is equal to %d", 0
    text db "A1l2e3x4a5r5e6m7e8r9e"
    len equ $-text; length of the given text above
    sum dd 0

segment code use32 class=code
start: 
    mov ecx, len
    cmp ecx, 0
    je finish

    Loop_text:
        xor eax, eax
        mov al, [text + ecx - 1]
        cmp al, '0'
        jl not_digit
        cmp al, '9'
        jg not_digit
        ; if it is a digit and the jump was not make, add it to the sum
        sub al, '0'
        add [sum], eax
        
        not_digit:
            loop Loop_text

    ;open file to be written in(by oppening with mode 'w' we also create it if it does not exist)
    push write
    push output_file
    call [fopen]
    add esp, 4*2

    cmp eax, 0
    je finish

    mov [file_desc], eax

    ;write in the file the sum
    push dword [sum]
    push print_text
    push dword [file_desc]
    call [fprintf]
    add esp, 4*3

    push dword [file_desc]
    call [fclose]
    add esp, 4

    finish:

	push dword 0 
	call [exit]
