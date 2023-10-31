; Signed representation
; x+a/b+c*d-b/c+e
; a,b,d-byte; c-word; e-doubleword; x-qword

bits 32
global start
extern exit
import exit msvcrt.dll

segment data use32 class=data
    a db 24
    b db -12
    d db -100
    c dw 6
    e dd 1000
    x dq 4500
    ;final result should be 4900 on a quad

segment code use32 class=code
start:
    
    push dword [x+4]
    push dword [x]
    pop EBX
    pop ECX
    
    ;since a is a byte and b is a byte, when we do a/b, a needs to be in a word(in AX)
    mov AL, [a]
    cbw; AL ->  AX
    idiv byte[b]; AH = AX % b and AL = AX / b (signed division)
    cbw; we convert AL(intiger part) to AX(signed conversion)
    cwde; we convert AX to EAX(signed conversion) 
    cdq; EAX -> EDX:EAX (signed conversion)
    
    ;we put x + a/b in ECX:EBX
    add EBX, EAX
    adc ECX, EDX; now we have a partial result in ECX:EBX
    
   ;we want to do c*d and since c is a word, we need to put d also in a dword and we wil get the result in EDX:EAX
   ;we will need to put d in DX:AX in order to be able to multiply it with c(dword)
   mov AL, [d]
   cbw; AL -> AX(signed conversion)
   imul word[c]; DX:AX = c*d = -600 | we need to put DX:AX in EDX:EAX (signed multiplication)
   push DX
   push AX
   pop EAX; we put DX:AX in EAX in order to be able to convert it to a quad to put the results in ECX:EBX
   cdq; EAX -> EDX:EAX (signed conversion)
   
   ;now we add (x+a/b) with (c*d) which means we add ECX:EBX with EDX:EAX
   add EBX, EAX
   adc ECX, EDX; now the partial result x+a/b+c*d is in ECX:EBX = 5102
   
   ;now we want to form b/c | since b is a byte and c is a word, we will need to put b in a dword first(AX). That dword will be DX:AX
   mov EAX, 0
   mov EDX, 0 ; reset the values of EAX and EDX for future operations
   mov AL, [b]
   cbw; AL -> AX(signed conversion)
   cwd; AX -> DX:AX(signed conversion)
   idiv word[c]; now we will have DX = b%c and AX = b/c | We will only use AX, the intiger part of the division!!!
   cwde; AX -> EAX(signed conversion)
   mov DX, 0; remove the remainder from the lower part of EDX to make the subtraction (we do this because we will need to subtract with borrow from ECX and the value from DX
   ; could change the result
   
   ;here we make the subtraction
   sub EBX, EAX
   sbb ECX, EDX; now we will have the partial result x+a/b+c*d-b/c in ECX:EBX
   
   ;to finish, we need to add e(dword) to the partial current result
   ;In order to do that, we first need to put e in a quad
   mov EAX, [e]
   cdq; EAX -> EDX:EAX(signed conversion)
   
   ;last operation(addition between ECX:EBX and EDX:EAX
   add EBX, EAX
   adc ECX, EDX

push dword 0
call [exit]