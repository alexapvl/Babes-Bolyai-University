; a - byte, b - word, c - double word, d - qword - Unsigned representation
; (a+b)-(a+d)+(c-a)

bits 32
global start
extern exit
import exit msvcrt.dll

segment data use32 class=data
    a db 100
    b dw 70
    c dd 325
    d dq 60

segment code use32 class=code
start:
    mov AL, [a]; AL <- a = 100 = 64h
    mov AH, 0; AL => AX = 100 = 0064h
    add AX, [b]; AX = AX + b(word) = a + b = 170 = 00AAh
    push word 0
    push AX
    pop EBX; now we have a+b in EBX
    mov ECX, 0; now we have a+b in ECX:EBX = 170 = 0000.0000:0000.00AAh
    
    mov AL, [a]; AL <- a = 100 = 64h
    mov AH, 0; AL => AX ... (unsigned conversion)
    push word 0; push 0000h in order to put 0064h after it in the stack to put it back in EAX with a pop
    push AX
    pop EAX
    mov EDX, 0; in case it has another value in it
    add EAX, dword[d]
    adc EDX, dword[d+4] ; completed the addition with possible carry in EDX:EAX = a + d = 160 = 0000.0000:0000.00A0h
    
    sub EBX, EAX
    sbb ECX, EDX; now we will have in EBX:ECX = (a+b)-(a+d)
    
    ;let's put c-a in EDX:EAX in order to be able to subtract it from ECX:EBX
    mov AX, [c]; AX <- c = 325 = 0145h
    mov DL, [a]; DL <- a =100 = 64h
    mov DH, 0; DX <- a = 100 = 0064h
    sub AX, DX; AX = AX-DX = c-a = 325 - 100 = 225 = 00E1h
    mov DX, 0
    push DX
    push AX
    pop EAX; we take from the stack the pair DX:AX and put it in EAX
    mov EDX, 0; we extend(unsigned) EAX to EDX:EAX = c-a = 325 - 100 = 225 = 00E1h
    
    ;make the subtraction EBX:ECX - EDX:EAX
    add EBX, EAX
    adc ECX, EDX; subtraction with possible borrow in order to get the correct result which will be EBX:ECX =(a+b)-(a+d)+(c-a) = 170 - 160 + 225 = 235 = 0000.0000:0000.00EBh

push dword 0
call [exit]
