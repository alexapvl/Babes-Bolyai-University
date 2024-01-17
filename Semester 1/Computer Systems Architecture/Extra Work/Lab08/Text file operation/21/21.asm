; A file name and a text (defined in the data segment) are given. 
; The text contains lowercase letters, digits and spaces. 
; Replace all the digits on odd positions with the character ‘X’. 
; Create the file with the given name and write the generated text to file.

; OBS: In my solution, I will take as odd positions, the even indexes from the string
; ex: in text: "A245H7" -> "A2X5H7" (4 was changed to 'X' since it is on an odd position in the text)

bits 32
global start

extern exit, fopen, fclose, fprintf
import exit msvcrt.dll
import fopen msvcrt.dll
import fclose msvcrt.dll
import fprintf msvcrt.dll

segment data use32 class=data
    text db "11111 111111 11111 11111", 0; 
    len equ $-text
    file db "output.txt", 0
    open_mode db "w", 0
    print_format db "%s", 0
    index dd -1
    file_desc dd -1

segment code use32 class=code
start:
    mov ecx, len
    cmp ecx, 0
    je skip_loop

    loop_text:
        mov al, [text+ecx-1]
        cmp al, '0'
        jl continue
        cmp al, '9'
        jg continue

        ;check if the digit is odd position
        ; we check only eax, not eax-1, see OBS above
        test ecx, 01h
        jz continue ; if ZF = 0 => number in ecx is even (least significant digit is 0)
        
        mov byte [text+ecx-1], 'X' ; if no jumps were made, change the character on that position to 'X'

        continue:
            loop loop_text

    skip_loop:
        push open_mode
        push file
        call [fopen]
        add esp, 4*2

        cmp eax, 0
        je finish
        mov [file_desc], eax

        push text
        push print_format
        push dword [file_desc]
        call [fprintf]
        add esp, 4*3

        push dword [file_desc]
        call [fclose]
        add esp, 4

    finish:

	push dword 0 
	call [exit] 
