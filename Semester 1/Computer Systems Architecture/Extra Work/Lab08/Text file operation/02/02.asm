; A text file is given. Read the content of the file, count the number of consonants 
; and display the result on the screen. 
; The name of text file is defined in the data segment.


bits 32
global start

extern exit, fopen, fclose, fread, printf
import exit msvcrt.dll
import fopen msvcrt.dll
import fclose msvcrt.dll
import fread msvcrt.dll
import printf msvcrt.dll

segment data use32 class=data
    result dd 0
    file db "input.txt", 0
    open_mode db "r", 0
    file_desc dd -1
    char db 0
    print_format db "No. of consonants in the file is %u", 0
    consonants db "BbCcDdFfGgHhJjKkLlMmNnPpQqRrSsTtVvWwXxYyZz"
    len equ $-consonants; length of string above

segment code use32 class=code
start:
    ;open file
    push open_mode
    push file
    call [fopen]
    add esp, 4*2
    cmp eax, 0
    je finish

    mov [file_desc], eax    

    read_from_file:
        push dword [file_desc]
        push dword 1
        push dword 1
        push char
        call [fread]
        add esp, 4*4

        cmp eax, 0
        je finish

        mov ecx, len
        loop_consonants:
            mov bl, [char]
            cmp bl, [consonants + ecx - 1]
            jne not_consonant

            inc dword [result]; if char is a consonant, increment the result by one

            not_consonant:
                loop loop_consonants
        
        jmp read_from_file

    finish:
        push dword [result]
        push print_format
        call [printf]
        add esp, 4*2

        push dword [file_desc]
        call [fclose]
        add esp, 4

	push dword 0 
	call [exit] 
