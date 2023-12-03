; A text file is given. Read the content of the file, determine the digit with the highest 
; frequency and display the digit along with its frequency on the screen. 
; The name of text file is defined in the data segment.
bits 32
global start

extern exit, fopen, fread, printf
import exit msvcrt.dll
import fopen msvcrt.dll
import fread msvcrt.dll
import printf msvcrt.dll

segment data use32 class=data
    file db "input.txt", 0; name of file
    file_mode db "r", 0; mode in which file is opened
    file_desc dd -1; address to the file
    digit db 0; the digit which we read from the file
    len equ 10; lenght of the frequency array
    frequency times len db 0; frequency array
    string db "The digit with the highest frequency is %d and it appears %d times.", 0; string to be printed
    result_digit dd 0; the digit with the highest frequency
    max_freq db 0; the frequency of the digit with the highest frequency

segment code use32 class=code
start:
    ; open file
    push file_mode
    push file
    call [fopen]
    add esp, 4*2

    ; check if file was opened with success
    cmp eax, 0
    je finish

    mov [file_desc], eax; we put the address of the file in the right place

    ;now we read from the file until we do not have any more digits remaining
    loop:
        ;read a digit
        ;fread(<address in which value read is stored>, <size bytes to be read at a time>, <number of times to read>, <address of file from which we read>)
        push dword [file_desc]
        push dword 1
        push dword 1
        push digit
        call [fread]
        add esp, 4*4

        cmp eax, 0; check to see if a digit was read
        je end_loop

        ; digit will be an ascii code from 30h - 39h
        ; we want to determine which digit it is by subtracting 30h = 48(decimal) from it
        mov ebx, 0
        mov bl, [digit]
        sub bl, 48; now al contains the digit
        ; now we want to increment the frequency of the digit
        mov al, [frequency + ebx]; get the frequency of the digit
        inc al; increment the frequency
        mov [frequency + ebx], al; move the frequency back in the array

        cmp al, [max_freq]; check to see if the frequency of the digit is higher than the max frequency
        jbe no_new_max; if we did not find a new maximum frequency, we loop
            mov [max_freq], al; if we found a new maximum frequency, we save it
            mov [result_digit], ebx; we save the digit with the highest frequency
        no_new_max:

        jmp loop
    end_loop:

    ; print the result
    push dword [max_freq]
    push dword [result_digit]
    push dword string
    call [printf]
    add esp, 4*3

    finish:
	push dword 0 
	call [exit] 