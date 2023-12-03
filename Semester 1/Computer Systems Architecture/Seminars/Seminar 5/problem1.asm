bits 32
global start        

extern exit, printf, scanf
import exit msvcrt.dll
import printf msvcrt.dll
import scanf msvcrt.dll 
                         
segment data use32 class=data
    m1 db "Give the first number: ", 13, 10, 0
	m2 db "Give the second number: ", 13, 10, 0
	rf db "%d", 0
	pf db "%d + %d is %d", 0
	n dd 0
	m dd 0 ;define as double word because we will need to push to the stack(convention)

segment code use32 class=code
    start:
        push m1 ; pushed address, so we pushed 4 bytes!!!
		; each time we push an address, we push 4 bytes(32 bits) at a time
		call [printf]
		add esp, 4
		
		;read first number
		push m ; pushed an address
		push rf ; pushed an address
		call [scanf]
		add esp, 4*2
		
		push m2
		call [scanf]
		add esp, 4
		
		;read second number
		push n
		push rf
		call [scanf]
		add esp, 4*2
		
		;add m and n
		mov ebx, [n]
		add ebx, [m]
		
		;print result
		push ebx
		push dword[m]
		push dword[n]
		push pf
		call [printf]
		add esp, 4*4
		
        push dword 0   
        call [exit]       
