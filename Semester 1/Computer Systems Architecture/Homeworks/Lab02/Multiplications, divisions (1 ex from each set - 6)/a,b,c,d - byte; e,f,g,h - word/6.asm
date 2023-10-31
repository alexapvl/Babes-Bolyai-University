bits 32

global start

extern exit
import exit msvcrt.dll

; (a*b)/c
segment data use32 class=data
    a db 19
    b db 1
    c db 6

segment data use32 class=code
    start:
        mov AL, [a] ; AL = a
        mov AH, [b] ; AH = b
        mul AH ; AX = AL * AH = a * b
        mov BX, AX ; BX = a * b
        mov AL, [c] ; AL = c
        cbw ; AL -> AX
        mov CX, AX ; CX = c
        mov AX, BX ; AX = a * b
        cwd ; AX -> DX:AX
        div CX ; AX = DX:AX / CX = (a * b) / c and DX = DX:AX % CX = (a * b) % c     

push dword 0
call [exit]