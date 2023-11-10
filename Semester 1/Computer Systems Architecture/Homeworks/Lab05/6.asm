; A byte string S is given. 
; Obtain the string D by concatenating the elements found on 
; the even positions of S and then the elements found on the odd positions of S.
; Example:
; S: 1, 2, 3, 4, 5, 6, 7, 8
; D: 1, 3, 5, 7, 2, 4, 6, 8


bits 32
global start

extern exit
import exit msvcrt.dll

segment data use32 class=data
	s db 1, 2, 3, 4, 5, 6, 7, 8 ; declare the array of bytes
	l equ $-s ; length of array S (in bytes)
	d times l db 0 ; declare the array of same length as array S

segment code use32 class=code 
start:
	mov ecx, l; counter for the first loop(we are looping through the elements at odd positions(not indexes)
	mov esi, 0; index in S
	mov edi, 0; index in D
	
	jecxz program_end; if the length of S is 0, we don't need to loop
	repeat_odd: ; first loop
		mov AL, [s + esi]
		
		test esi, 1
		; test 11111110, 000001 <=> 11111110 & 0000001 = 0 => even => ZERO FLAG is 1
		; test 11111111, 000001 <=> 11111111 & 0000001 = 1 => odd => ZERO FLAG IS 0
		jnz not_even
		
		mov [d + edi], al
		inc edi
		
	not_even:
		inc esi
		loop repeat_odd
		
		
	mov ecx, l
	mov esi, 1; first even position in S
	
	repeat_even:
		mov al, [s + esi]
		
		test esi, 1
		jz not_odd
		
		mov [d + edi], al
		inc edi
		
	not_odd:
		inc esi
		loop repeat_even
	
	program_end:
	
push dword 0
call [exit]