; a - byte, b - word, c - double word, d - qword - Signed representation
; c-(d+a)+(b+c)

bits 32
global start
extern exit
import exit msvcrt.dll

segment data use32 class=data
    a db 100
    b dw -100
    c dd -300
    d dq 500
    ; result should be -1300 (signed)
segment code use32 class=code
start:
    ; put c into a quad in order to be able to subtract (d+a) which will also be a quad
    mov EAX, [c] 
    cdq; c is now in the quad EDX:EAX and we will move it in ECX:EBX
    push EDX
    push EAX
    pop EBX
    pop ECX ; with theese 4 instructions we copied the value of c in ECX:EBX = -300 (signed)
    
    ;we put a into a quad to add it with d(also a quad)
    mov AL, [a]
    cbw; now AX = a
    cwde; now EAX = a
    cdq; now EDX:EAX = a = 100(SIGNED REPRESENTATION!!!!!!!)
    add EAX, dword[d]
    adc EDX, dword[d+4]; EDX:EAX = d+a = 600
    
    ;we make the subtraction c-(d+a), in other words we will make this subtraction: ECX:EBX - EDX:EAX
    sub EBX, EAX
    sbb ECX, EDX ; now we have c-(d+a) in ECX:EBX = -900
    
    ;we now want to put in EDX:EAX the last operation (b+c)
    mov EAX, 0
    mov AX, [b]
    cwde; EAX = b
    add EAX, [c]; EAX = b+c
    cdq; EDX:EAX = b+c = -400
    
    ;we want to make the last addition which will give us the final result
    ;since in ECX:EBX we have c-(d+a) and in EDX:EAX we have (d+c), we will add those together
    add EBX, EAX
    adc ECX, EDX ; now the final result will be in ECX:EBX


push dword 0
call [exit]