; Read two numbers a and b (in base 10) from the keyboard and calculate their product. 
; This value will be stored in a variable called "result" (defined in the data segment).
bits 32
global start
extern exit, printf, scanf

import exit msvcrt.dll
import printf msvcrt.dll
import scanf msvcrt.dll

segment data use32 class=data
    number1 dd 0
    number2 dd 0
    result dd 0
    string1 db "Enter first number: ", 0
    format1 db "%d", 0
    string2 db "Enter second number: ", 0
    format2 db "%d", 0
    string3 db "Result: %d", 0

segment code use32 class=code
start:
    ;printf("Enter first number: ")
    push string1
    call [printf]
    add esp, 4

    ;scanf("%d", &number1)
    push number1
    push format1
    call [scanf]
    add esp, 8

    ;printf("Enter second number: ")
    push string2
    call [printf]
    add esp, 4

    ;scanf("%d", &number2)
    push number2
    push format2
    call [scanf]
    add esp, 8

    ;result = number1 + number2
    mov eax, [number1]
    add eax, [number2]
    mov [result], eax

    ;printf("Result: %d", result)
    push dword[result]
    push string3
    call [printf]
    add esp, 8

    ;exit(0)
    push dword 0
    call [exit]
