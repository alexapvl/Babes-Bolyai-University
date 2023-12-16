bits 32
global start

import scanf msvcrt.dll
import printf msvcrt.dll
import exit msvcrt.dll

extern scanf, printf, exit

extern concatenate

segment data use32 class=data
    msg1 db "Enter the first string: ", 0
    msg2 db "Enter the second string: ", 0
    msg3 db "Enter the third string: ", 0
    msg db "The concatenated string is: %s", 13, 10, 0
    format db "%s", 0
    str1 resb 10
    str2 resb 10
    str3 resb 10
    result resb 30

segment code use32 class=code
start:
    ;read first string
    push msg1
    call [printf]

    push str1
    push format
    call [scanf]

    ;read second string
    push msg2
    call [printf]

    push str2
    push format
    call [scanf]

    ;read third string
    push msg3
    call [printf]

    push str3
    push format
    call [scanf]

    ;concatenate strings using the concatenate.asm module
    push str3
    push str2
    push str1
    push result
    call concatenate
    add esp, 4*4

    push result
    push msg
    call [printf]

push dword 0
call [exit]