bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

; our data is declared here (the variables needed by our program)
; (a-b)+(d-c) -> a,b,c,d are all bytes
segment data use32 class=data
    a db 3 ; a = 3 = 03h
    b db 4 ; b = 4 = 04h
    c db 5 ; c = 5 = 05h
    d db 6 ; d = 6 = 06h

; our code starts here
segment code use32 class=code
    start:
        mov AL, [a] ; AL = a
        sub AL, [b] ; AL = a - b
        mov AH, [d]
        sub AH, [c] ; AH = d - c
        add AL, AH ; AL = AL + AH = (a - b) + (d - c)
    
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
