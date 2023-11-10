; A word string s is given. Build the byte string d such that each element d[i] contains:
; - the count of zeros in the word s[i], if s[i] is a negative number
; - the count of ones in the word s[i], if s[i] is a positive number

; Example: 
; s: -22, 145, -48, 127
; In binary:
; 1111111111101010, 10010001, 1111111111010000, 1111111
; => d: 3, 3, 5, 7

bits 32
global start

extern exit
import exit msvcrt.dll

segment data use32 class=data
	s dw -22, 145, -48, 127
	l equ ($-s)/2; because s is a string of words
	d times l db 0; reserve in memody a byte for each word in string s
	
segment code use32 class=code
	start:
		mov ecx, l; number of elements that need to be verified
		mov esi, s
		mov edi, d; in ESI and EDI we put the OFFSET of strings s and d 
		cld; clear direction flag
		
		jecxz End
		Repeat:
			;we need to move the word s[i] to AX
			lodsw; <=> mov AX, [s + esi] | esi += 2 (word => +2)
			mov bh, 0; here we will count the number of zeros/ones of the number in s[i]
			mov bl, 16; the number of shifts we will need in order to check every bit from s[i]
			;check if s[i] is negative or positive => signed comparation (greater or less in jump instruction)
			cmp ax, 0
			
			jge positive
				negative:
				;s[i] is negative, so we count the number of zeros in the word(16 bits) => 16 shifts to the left/right
				shl ax, 1; CF will be set to 0 or 1, depending on the most significant bit that was shifted out of representation
				jc skip1; jump because we need to count the zeros
					inc bh
				skip1:
				dec bl
				cmp bl, 0
			jnz negative
				
			jmp skip_positive	
			
			positive: ;if s[i] is positive, we jump here
				;s[i] is positive, so we count the number of ones
				shl ax, 1
				jnc skip2; jump because we need to count the ones
					inc bh
				skip2:
				dec bl
				cmp bl, 0
			jnz positive	
				
			skip_positive:
			mov al, bh; prepare for stosb instruction
			stosb; <=> mov AL, [d+edi] | edi += 1 (byte => +1)
			
		loop Repeat
		End:
	
push dword 0
call [exit]