bits 32

global start

extern exit
import exit msvcrt.dll

; [2*(a+b)-5*c]*(d-3) - a,b,c - byte; d - word
segment data use32 class=data
    a db 20
    b db 5
    c db 3
    d dw 103

segment data use32 class=code
    start:
        mov AL, [a] ; AL = a
        add AL, [b] ; AL = a + b
        mov AH, 2 ; AH = 2 = 02h
        mul AH ; AX = AL * AH = (a + b) * 2
        mov BX, AX ; BX = (a + b) * 2
        mov AL, [c] ; AL = c
        mov AH, 5 ; AH = 5 = 05h
        mul AH ; AX = AL * AH = 5 * c
        sub BX, AX ; BX = (a + b) * 2 - (5 * c)
        mov AX, BX ; AX = (a + b) * 2 - (5 * c) - moved in order to be able to multiply later with (d - 3)
        mov BX, [d] ; BX = d
        sub BX, 3 ; BX = d - 3
        mul BX ; DX:AX = AX * BX = ((a + b) * 2 - (c * 5)) * (d - 3)
        push DX
        push AX ; we put DX and AX on the stack because we want the final result to be in EAX so it's easier to verify
        pop EAX ; EAX will have the final result of the ecuation: ((a + b) * 2 - (c * 5)) * (d - 3)
    
push dword 0
call [exit]