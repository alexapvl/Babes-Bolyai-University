;Given the bytes a and b, compute the doubleword c as follows:
;	the bits 31-16 of C have the value 1
;	the bits 3-0 of C are the same as the bits 6-3 of B
;	the bits 7-4 of C have the value 0
;	the bits 10-8 of C have the value 110
;	the bits 15-11 of C are the same as the bits 4-0 of A

bits 32
global start
extern exit
import exit msvcrt.dll


segment data use32 class=data
	a db 01010111b; a = 0101 0111b = 57h
	b db 10111110b;  b = 1011 1110b = BEh
	c dd 0

segment code use32 class=code
	start:
	mov EBX, 0; here we will store the partial results and at the end, we will move the value from EBX to the variable c(dword)
	
	;the bits 31-16 of C have the value 1:
	or EBX, 11111111111111110000000000000000b; EBX = 1111 1111 1111 1111 0000 0000 0000 0000b
	
	;the bits 3-0 of C are the same as the bits 6-3 of B
	mov AL, [b]; AL = 1011 1110b
	and AL, 01111000b; AL = 0011 1000b
	shr AL, 3; AL = 0000 0111b
	or BL, AL; EBX = 1111 1111 1111 1111 0000 0000 0000 0111b
	
	;the bits 7-4 of C have the value 0
	;those bits already have the value 0 (bits 7-4 from EBX)
	;EBX = 1111 1111 1111 1111 0000 0000 0000 0111b
	
	;the bits 10-8 of C have the value 110
	;we make the operation only on BX because we don't need the whole EBX to make this insertion of 3 bits
	or BX, 0000011000000000b; EBX = 1111 1111 1111 1111 0000 0110 0000 0111b
	
	;the bits 15-11 of C are the same as the bits 4-0 of A
	mov AX, 0; reset the value of EAX
	mov AL, [a]; AX = 0000 0000 0101 0111b
	shl AX, 11; We move bits 4-0 to 15-11, no need to zero anything since we will have only zeros on the right
	;AX = 1011 1000 0000 0000b
	or BX, AX; BX = 1011 1110 0000 0000b
	
	mov [c], EBX; answer should now be in c(dword)
	
push dword 0
call [exit]