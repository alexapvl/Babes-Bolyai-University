;Given the word A, obtain the integer number N represented on the bits 2-0 of A. 
;Then obtain the word B by rotating A n positions to the right. Compute the doubleword C:
;the bits 15-8 of C have the value 0
;the bits 23-16 of C are the same as the bits of 9-2 of B
;the bits 31-24 of C are the same as the bits of 14-7 of A
;the bits 7-0 of C have the value 1
bits 32
global start
extern exit
import exit msvcrt.dll

segment data use32 class=data
	a dw 0000000001010111b; a = 0000 0000 0101 0111b = 57h
	b dw 0; will be obtained by rotating 'a' with 'n' positions | in our case b = 1010 1110 0000 0000b
	n dw 0; number of positions with which we rotate number "a"(word) | in our case n = 7
	c dd 0; the result will be formed in EBX and moved, at the end, in 'c'

segment code use32 class=code
start:
	;obtain the integer number n represented on the bits 2-0 of A
	mov AX, [a]; we put the value a in AX
	and AX, 0000000000000111b; used mask to isolate bits 2-0 from 'a'
	mov [n], AX; now we have the value from bits 2-0 from 'a' in 'n' => n = 7
	
	;obtain the word B by rotating A with 'n' positions to the right
	mov AX, [a]; we put 'a' in AX in order to rotate it and the n we put the result in 'b'
	mov CL, byte[n]; since only the first 3 bits from 'n' are needed, we put the lower part in CL
	ror AX, CL; we rotate to the right the value of 'a' a number of 'n' times
	mov [b], AX; we put the rotated value in 'b'
	
	;the bits 15-8 of 'c' have the value 0
	;they already have the value 0 because we initialized 'c' with 0
	
	;the bits 23-16 of 'c' are the same as the bits of 9-2 of 'b'
	;first we will put the value of 'b'(word) in EAX(doubleword)
	mov EAX, 0
	mov AX, [b]
	;then, we will shift to the left the bits 9-2 from EAX to bits 23-16 | this will mean 14 bits
	shl EAX, 14
	;after this, we will isolate the bits 23-16 in order to prepare them to be inserted in EBX
	and EAX, 00000000111111110000000000000000b; bits 23-16 are now isolated and ready to be inserted in EBX
	mov EBX, 0
	or EBX, EAX
	
	;the bits 31-24 of 'c' are the same as the bits of 14-7 of 'a'
	;first we will put the value of 'a'(word) in EAX(doubleword)
	mov EAX, 0
	mov AX, [a]
	;then, we will shift the bits 14-7 to bits 31-24 | this will mean 17 bits
	shl EAX, 17
	;then we need to isolate those bits (31-24)
	and EAX, 11111111000000000000000000000000b
	;insert the result in EBX
	or EBX, EAX
	
	;the bits 7-0 of C have the value 1
	or BL, 11111111b; inserted value of all bits to be 1 in the lower part of EBX which means the bits 7-0 from EBX
	
	;move the value from EBX to 'c'
	mov [c], EBX
	
	;in our case, the final result should be: c = 0000 0000 1000 0000 0000 0000 1111 1111b = 008000FFh
	
push dword 0
call [exit]
