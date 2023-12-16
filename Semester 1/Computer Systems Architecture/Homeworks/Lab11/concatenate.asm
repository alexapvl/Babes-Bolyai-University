bits 32
segment code use32 public code 
global concatenate

concatenate:
    ; put in esi the address of the result param from memory
    mov edi, [esp+4]

    ;we take each string one by one and put each character
    ;string1
    mov esi, [esp+8]
    cld
    string1:
        cmp byte [esi], 0
        je end_string1
        movsb
        jmp string1

    end_string1:
        mov esi, [esp+12]

    cld
    string2:
        cmp byte [esi], 0
        je end_string2
        movsb
        jmp string2

    end_string2:
        mov esi, [esp+16]
    
    cld
    string3:
        cmp byte [esi], 0
        je final
        movsb
        jmp string3
    
    final:
        mov byte [edi], 0
        ret
        