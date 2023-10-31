bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

; our data is declared here (the variables needed by our program)
; (a+b)-(a+d)+(c-a), a,b,c,d - bytes
segment data use32 class=data
    a db 1 ; a = 1 = 01h
    b db 2 ; b = 2 = 02h
    c db 3 ; c = 3 = 03h
    d db 4 ; d = 4 = 04h

; our code starts here
segment code use32 class=code
    start:
        mov AL, [b]
        add AL, [a] ; AL = a + b
        mov AH, [d]
        add AH, [a] ; AH = a + d
        mov BL, [c]
        sub BL, [a] ; BL = c - a
        sub AL, AH ; AL = (a + b) - (a + d)
        add AL, BL ; AL = AL + (c - a) = (a + b) - (a + d) + (c - a)
        
    
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
