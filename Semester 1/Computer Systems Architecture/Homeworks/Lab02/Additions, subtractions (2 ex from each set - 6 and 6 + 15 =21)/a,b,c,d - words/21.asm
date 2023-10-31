bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

; our data is declared here (the variables needed by our program)
; a-c+d-7+b-(2+d)
segment data use32 class=data
    a dw 123 ; a = 123
    b dw 100 ; b = 100
    c dw 98 ; c = 98
    d dw -100 ; d = -100
    seven QEU 7
    two QEU 2

; our code starts here
segment code use32 class=code
    start:
        mov AX, [a] ; AX = 123 = 
        sub AX, [c] ; AX = AX - 98 = 123 - 98 = 25 = 0019h
        add AX, [d] ; AX = AX + (-100) = AX - 100 = 25 - 100 = -75 = 
        add AX, seven ; AX = AX + 7 = -75 + 7 = -68 = 
        add AX, [b] ; AX = AX + 100 = -68 + 100 = 32 = 0020h
        sub 
    
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
