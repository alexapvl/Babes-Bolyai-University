; move elements from a file to another, each char + 1
bits 32
global start

extern fopen, fclose, fread, fwrite, perror, exit
import fopen msvcrt.dll
import fclose msvcrt.dll
import fread msvcrt.dll
import fwrite msvcrt.dll
import perror msvcrt.dll
import exit msvcrt.dll

segment data use32 class=data
    input_file db "file.txt", 0 ; input file
    input_mode db "r", 0        ; input mode
    file_desc_input dd -1 ;file descriptor input
    output_file db "out.txt", 0  ; output file
    output_mode db "w", 0        ; output mode
    file_desc_output dd -1 ; file descriptor output
    e db "error", 0
    c db 0

segment code use32 class=code
    start:
        push input_mode
        push input_file
        call [fopen] ; fopen(<file_name>, <access_mode>)
        add esp, 4*2
        
        cmp eax, 0
        je end1
        mov [file_desc_input], eax; file descriptor(address) will be in eax and we will put it in the right place

        push output_mode
        push output_file
        call [fopen]
        add esp, 4*2
        
        cmp eax, 0
        je end2
        mov [file_desc_output], eax

        loop1:
            push dword[file_desc_input]
            push dword 1
            push dword 1
            push c
            call [fread]
            add esp, 4*4
            
            cmp eax, 0; in eax will be the number of elements which were read from the file
            je end_loop1

            inc byte[c]
            push dword[file_desc_output]
            push dword 1
            push dword 1
            push c
            call [fwrite]
            add esp, 4*4
            
        jmp loop1
        end_loop1:
        
        push dword[file_desc_output]
        call [fclose]
        add esp, 4
        
        jmp ok
        end2:
            push e
            call [perror]
            add esp, 4
        ok:
            push dword[file_desc_input]
            call [fclose]
            add esp, 4
            jmp ok2
        end1:
            push e
            call [perror]
            add esp, 4
        ok2:
        
        push dword 0
        call [exit]