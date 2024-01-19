; Read the number N from the keyboard, 0<N<7
; Read numbers from the keyboard until "$" is read
; Create and write in the file "numbers2.txt" all the positive numbers which have the 
; second decimal digit(from right to left) equal to N
; ex: N = 3
; numbers read are: 31 -234 9874 4631 5543 -734 335
; numbers written in file should be: 31 4631 335

bits 32
global start        

extern exit, fopen, fclose, scanf, fprintf, strlen
import exit msvcrt.dll
import fopen msvcrt.dll
import fclose msvcrt.dll
import scanf msvcrt.dll
import fprintf msvcrt.dll  
import strlen msvcrt.dll 
import printf msvcrt.dll  
   
segment data use32 class=data
	N dd -1
    file db "numbers2.txt", 0
    mode db "w", 0
    descriptor dd -1
    format db "%d", 0
	scan_format db "%s", 0
	print_format db "%s ", 0
    x dd 0

segment code use32 class=code
    start:
		;read N
		push dword N
		push dword format
		call [scanf]
		add esp, 4*2
		
		add byte [N], '0'
		mov bl, byte [N]; move the character in BL to be able to compare it later
		
		; open the file in write mode and put the descriptor in the right place
		push dword mode
		push dword file
		call [fopen]
		add esp, 4*2
		cmp eax, 0
		je finish
		mov [descriptor], eax
		
		cont_read:
			;read a number
			push x
			push scan_format
			call [scanf]
			add esp, 4*2
			; x was read as a string
			
			cmp byte[x], "$"
			je finish; finish read
			
			cmp byte[x], "-"
			je cont_read; if negative, skip
			
			push x
			call [strlen]
			add esp, 4*1
			;EAX = len(x)
			
			cmp [x+EAX-2], bl; second digit from the right
			jne cont_read; if not equal, do not print it
			
			;if after all verifications, no jumps were performed, print the number in the file
			push dword x
			push dword print_format
			push dword [descriptor]
			call [fprintf]
			add esp, 4*3
				
		jmp cont_read
			
		finish:
			push dword [descriptor]
			call [fclose]
			add esp, 4*1
        
        push dword 0
        call [exit]
