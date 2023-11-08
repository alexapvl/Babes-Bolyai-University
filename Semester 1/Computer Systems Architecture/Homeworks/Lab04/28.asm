;Given the quadword A, obtain the integer number N represented on the bits 19-17 of A. 
;Then obtain the the doubleword B by rotating the high doubleword of A N positions to the left. 
;Obtain the byte C as follows:
;	-the bits 2-0 of C are the same as the bits 11-9 of B
;	-the bits 7-3 of C are the same as the bits 24-20 of B

bits 32
global start
extern exit
import exit msvcrt.dll

segment data use32 class=data
    a dq 00ABCDEF00ABCDEFh
    n db 0
    b dd 0
    c db 0

segment code use32 class=code
start:
    ;Given the quadword A, obtain the integer number N represented on the bits 19-17 of A.
    ;A = 00ABCDEF00ABCDEFh = 0000 0000 1010 1011 1100 1101 1110 1111 0000 0000 1010 1011 1100 1101 1110 1111
    ;desired bits are from the first B from right to left, bits 3-1 ----------------|||---------------
    ;because we only need the bits 19-17 from A, then we only need the lower part of A(bits 31-0)
    mov EAX, dword[a]; EAX = lower_part(A)
    ;isolate bits 19-17 from EAX
    and EAX, 00000000000011100000000000000000b
    ;EAX = 0000 0000 0000 1010 0000 0000 0000 0000b = 000A0000h
    ;we need to put that number on 3 bits in N
    ;to do so, we will shift the 3 bits(19-17) to the beginning of EAX, in AL to be more precise => bits 2-0 => shr EAX, 17
    shr EAX, 17; EAX = 0000 0000 0000 0000 0000 0000 0000 0101b = 00000005h 
    ;after that, we will move the value from AL in N
    mov [n], AL; N = 0000 0101b = 05h

    ;Then obtain the the doubleword B by rotating the high doubleword of A N positions to the left.
    ;we move the high doubleword of A in EAX
    mov EAX, dword[a+4]; EAX = 0000 0000 1010 1011 1100 1101 1110 1111b = 00ABCDEFh
    ;then, we rotate EAX N positions to the left
    mov CL, [n]
    rol EAX, CL; EAX = 0001 0101 0111 1001 1011 1101 1110 0000b = 1579BDE0h
    ;move value of EAX in doubleword B
    mov [b], EAX; B = 0001 0101 0111 1001 1011 1101 1110 0000b = 1579BDE0h

    ;The bits 2-0 of C(byte) are the same as the bits 11-9 of B
    ;Bits 11-9 from B can be accessed from the lower word of B(bits 11-0)
    ;so we will move that value into a word, like AX
    mov AX, word[b]; AX = 1011 1101 1110 0000b = BDE0h
    ;isolate bits 11-9 --------|||-----------
    and AX, 0000111000000000b; AX = 0000 1100 0000 0000b = 0C00h
    ;shift bits 11-9 to bits 2-0(location in C) => shift by 9 bits to the right
    shr AX, 9; AX = 0000 0000 0000 0110b = 0006h
    ;insert bits in C(byte) using AL(byte)
    or [c], AL; C = 0000 0110b = 0006h

    ;The bits 7-3 of C are the same as the bits 24-20 of B
    ;Same strategy as above but now we work with the higher word of B
    ;We move the high word of B into AX and bits 24-20 will be (-16) => 8-4
    mov AX, word[b+2]; AX = 0001 0101 0111 1001b
    ;isolate bits 8-4(24-20 from B)-|-||||-----
    and AX, 0000000111110000b; AX = 0000 0001 0111 0000b = 0170h
    ;shift bits in 8-4 to bits 7-3(location in C) => shift 1 bit to the right 
    shr AX, 1; AX = 0000 0000 1011 1000b = 00B8h
    ;insert bits in C(byte) using AL(byte)
    or [c], AL; C = 1011 1110b = BEh

push dword 0
call [exit]