bits 32
global sum

segment code use32 class=code
    sum:
        ; stack
        ; ret address   - esp
        ; a             - esp+4
        ; b             - esp+8
        mov eax, [esp+4]; eax = a
        add eax, [esp+8]; eax = a + b
    
    ret 8 ; free 8 bytes from the stack