bits 32
global calculate

segment data use32 class=data
    check db 0
    ; will be used to see if the number of set bits in the first byte(al) is greater than in the second(bl)
    ; if we find a set bit in the first byte we increment by one "check"
    ; in the second one we decrement
    ; at the end if check is negative, we put the value 1 in AH and 0 otherwise

segment code use32 class=code
    calculate:
        mov ecx, 8; first loop will go through the byte in al
        first_loop:
            shl al, 1; CF = most significant bit of al
            jnc skip_1
            inc byte [check]
            skip_1:
        loop first_loop

        mov ecx, 8; second loop will go through bl
        second_loop:
            shl bl, 1
            jnc skip_2
            dec byte [check]
            skip_2:
        loop second_loop

        cmp byte [check], 0
        jge make_no_swap
        mov ah, 1; make swap
        jmp finish_calculate

        make_no_swap:
            mov ah, 0
        
        finish_calculate: 
            ret
