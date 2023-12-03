; A string of doublewords is given. 
; Compute the string formed by the high bytes of the low words from the elements of the doubleword string and these bytes should be multiple of 10.
; Example:
; given the doublewords string:
; s DD 12345678h, 1A2B3C4Dh, FE98DC76h 
; obtain the string
; d DB 3Ch, DCh.
bits 32
global start        

extern exit
import exit msvcrt.dll

segment data use32 class=data
    s DD 12345678h, 1A2B3C4Dh, 98FEDC76h
    len equ ($-s)/4
    ten db 10
    d times len db 0

segment code use32 class=code
    start:
        mov esi, s
        cld
        mov ecx, len
        mov edi, d
        repeta:
            LODSW ;load the lower word
            mov BH, AH ;store the value of the higher byte in BH
            LODSW ;load the higher word; this is only done to increment the source index, we do not use it
            mov AL, BH ;bring back the value stored in BH
            mov AH, 0 ; convert unsigned AL -> AX
            div byte [ten] ; divide AX(word) by a byte
            cmp AH, 0 ;check if the remainder is 0
            jnz nonmultiple ;if it is not 0, do not do anything
            mov AL, BH ;if remainder is zero, overwrite the quotient with the original value stored in BH
            STOSB ;store byte(implicit AL) in destination string and increment destination index(edi)
            nonmultiple:
        loop repeta
    
        push dword 0
        call [exit]