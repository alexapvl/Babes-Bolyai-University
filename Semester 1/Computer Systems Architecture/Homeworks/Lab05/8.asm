; A character string S is given. Obtain the string D that contains all capital letters of the string S.

; S: 'a', 'A', 'b', 'B', '2', '%', 'x', 'M'
; D: 'A', 'B', 'M'

; OBS: A character is upper case if 41h <= char < 61h, where 41h = 'A' and 61h = 'a'
; 'A' -> 41h = 65
; 'a' -> 61h = 97

bits 32
global start

extern exit
import exit msvcrt.dll

segment data use32 class=data
	s db 'a', 'A', 'b', 'B', '2', '%', 'x', 'M'
	l equ $-s
	d times l db 0

segment code use32 class=code
start:
	mov ecx, l
	mov esi, 0; index in source array
	mov edi, 0; index in destination array
	
	jecxz program_end
	
	repeat:
		mov al, [s + esi]; we have the value of the current character in AL
		; we need to check if the character is in the given above range
		
		cmp al, 65
		jb skip
			cmp al, 97
			jae skip
			mov [d + edi], al
			inc edi
		
		skip:
			inc esi
	
	loop repeat

	program_end:

push dword 0
call [exit]