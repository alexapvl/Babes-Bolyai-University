; A text file is given. The text file contains numbers (in base 10) separated by spaces. 
; Read the content of the file, determine the maximum number 
; (from the numbers that have been read) and write the result at the end of file.


bits 32
global start

extern exit, fopen, fscanf, fprintf, fclose
import exit msvcrt.dll
import fopen msvcrt.dll
import fscanf msvcrt.dll
import fprintf msvcrt.dll
import fclose msvcrt.dll

; I use fscanf in order to read whole numbers at a time, regardless of their number of digits, since they are 

segment data use32 class=data
    file db "file.txt", 0
    open_mode db "a+", 0
    file_desc dd -1
    maxi dd 080000000h; very small number
    number dd 0
    read_format db "%d", 0
    write_in_file_format db " %d", 0

segment code use32 class=code
start:
    push open_mode
    push file
    call [fopen]
    add esp, 4*2

    cmp eax, 0
    je finish

    mov [file_desc], eax

    read_number:
        push number
        push read_format
        push dword [file_desc]
        call [fscanf]
        add esp, 4*3

        cmp eax, 1; if fscanf read something from the file, the value of eax will be set to 1
        jne finish

        mov ebx, [number]
        cmp ebx, [maxi]
        jle no_change

        mov [maxi], ebx

        no_change:
            jmp read_number

    finish:
        push dword [maxi]
        push write_in_file_format
        push dword [file_desc]
        call [fprintf]
        add esp, 4*3

        push dword [file_desc]
        call [fclose]
        add esp, 4

	push dword 0 
	call [exit] 
