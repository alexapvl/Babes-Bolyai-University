; A text file is given. 
; Read the content of the file, count the number of vowels and display the result on the screen. 
; The name of text file is defined in the data segment.

bits 32
global start

import exit msvcrt.dll
import fopen msvcrt.dll
import fread msvcrt.dll
import printf msvcrt.dll
import fclose msvcrt.dll

extern exit, fopen, fread, printf, fclose

segment data use32 class=data
	file db "input.txt", 0
	access_mode db "r", 0
	char db 0
	result dd 0
	print_format db "No. of vowels is equal to %u", 0
	vowels db "AaEeIiOoUu"
	len equ $-vowels
	file_desc dd -1

segment code use32 class=code
start:
	; open the file and get the file descriptor
	push access_mode
	push file
	call [fopen]
	add esp, 4
	cmp eax, 0
	je finish

	mov [file_desc], eax

	read_from_file:
		push dword [file_desc]
		push dword 1
		push dword 1
		push char
		call [fread]
		add esp, 4*4
		
		cmp eax, 0
		je finish

		mov ecx, len; should compare ecx to 0 but we know for a fact that ecx will always be 10 before entering the loop
		loop_vowels:
			mov bl, [char]
			cmp bl, [vowels + ecx - 1]
			jne not_vowel

			inc dword [result]; if char is a vowel, increment the result by one

			not_vowel:
				loop loop_vowels

		jmp read_from_file


	finish:
		push dword [result]
		push print_format
		call [printf]
		add esp, 4*2

		push dword [file_desc]
		call [fclose]
		add esp, 4

	
	push dword 0 
	call [exit] 
