%ifndef print_32_included
%define print_32_included

[bits 32]

VIDEO_MEMORY equ 0xB8000
VIDEO_MEMORY_LIMIT equ 0xB9280
WHITE_ON_BLACK equ 0x0E ; the color byte for each character

print:
    pusha
    mov edx, [cureent_char]
	
	print_loop:
	
    mov al, [ebx]
    mov ah, WHITE_ON_BLACK

    cmp al, 0
    je print_done

    mov [edx], ax
    add ebx, 1 ; next char
    add edx, 2 ; next video memory position
	
    jmp print_loop

	print_done:
	mov [cureent_char], edx
    popa
    ret

print_nl:
	pusha
	
	mov ebx, cureent_char
	mov eax, [ebx]
	sub eax, VIDEO_MEMORY
	mov ecx, 160
	
	xor edx, edx  ; set to zero
	div ecx
	
	mov eax, 160
	sub eax, edx
	add eax, [ebx]
	mov [ebx], eax
	
	popa
	ret

clear_screen:
	pusha
	mov ebx, VIDEO_MEMORY
	xor eax, eax  ; set to zero
	
	clear_screen_loop:
	cmp ebx, VIDEO_MEMORY_LIMIT
	je clear_screen_done
	mov [ebx], eax
	add ebx, 2
	jmp clear_screen_loop
	
	clear_screen_done:
	popa
	ret

; get data from edx, number of digits to print from esi
print_hex:
	pusha
	xor ecx, ecx  ; set to zero

	print_hex_loop:
	
	cmp ecx, esi
	je print_hex_end
	
	mov eax, edx
	and eax, 0xF
	add al, 0x30
	cmp al, 0x39
	jle print_hex_continue
	
	add al, 7
	
	print_hex_continue:
	
	mov ebx, print_hex_mem+7
	sub ebx, ecx
	mov [ebx], al
	
	ror edx, 4
	inc ecx
	
	jmp print_hex_loop

	print_hex_end:
	
	mov ebx, print_hex_mem+8
	sub ebx, esi
	call print
	popa
	ret

; get data from edx, number of digits to print from esi
print_integer:
	pusha
	mov eax, edx
	xor ecx, ecx
	
	print_integer_loop:
	
	cmp eax, 0
	je print_integer_end
	
	mov ebx, 10
	xor edx, edx ; set to zero, needed for division
	div ebx
	
	add dl, 0x30  ; reminder of division
	mov ebx, print_integer_mem+9
	sub ebx, ecx
	mov [ebx], dl
	inc ecx
	
	jmp print_integer_loop
	
	print_integer_end:
	
	mov ebx, print_integer_mem+10
	sub ebx, ecx
	call print
	
	popa
	ret
	
; get data from edx
print_binary:
	pusha
	xor ecx, ecx  ; set to zero

	print_binary_loop:
	
	cmp ecx, 32
	je print_binary_end
	
	mov eax, edx
	and eax, 1
	add al, 0x30
	
	mov ebx, print_binary_mem+31
	sub ebx, ecx
	mov [ebx], al
	
	ror edx, 1
	inc ecx
	
	jmp print_binary_loop

	print_binary_end:
	
	mov ebx, print_binary_mem+32
	shl esi, 2
	sub ebx, esi
	call print
	
	popa
	ret
	
	
print_hex_mem: db "00000000", 0
print_integer_mem: db "0000000000", 0
print_binary_mem: times 32 db "0"
db 0
cureent_char: dd 0xb8000


%endif ; print_32_included