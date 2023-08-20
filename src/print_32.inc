%ifndef print_32_included
%define print_32_included

[bits 32]

%define VIDEO_MEMORY 0xB8000
%define VIDEO_MEMORY_LIMIT 0xB8FA0
%define VIDEO_COLS 80
%define VIDEO_ROWS 25
%define CHAR_ATTRIBUTE 0x0E

print_hex_mem: db "00000000", 0
print_integer_mem: db "0000000000", 0
print_binary_mem: times 32 db "0"
db 0

cureent_char: dd 0xb8000
cureent_x: dd 0
cureent_y: dd 0

;-------------------------------------
;   Print string pointed by EBX
;-------------------------------------
print:
    pushad

    mov edx, [cureent_char]
	mov ah, CHAR_ATTRIBUTE
	
	print_loop:

	cmp edx, VIDEO_MEMORY_LIMIT ; wrapp around the buffer

	jl print_continue
	mov edx, VIDEO_MEMORY

	print_continue:
	
    mov al, [ebx]
    cmp al, 0

    je print_done

    mov [edx], ax
    add ebx, 1 ; next char
    add edx, 2 ; next video memory position
	
    jmp print_loop

	print_done:
	mov [cureent_char], edx

	xor edx, edx
	mov eax, [cureent_char]
	sub eax, VIDEO_MEMORY
	mov ecx, VIDEO_COLS * 2  ; width of screen (in bytes)

	div ecx
	mov [cureent_x], edx  ; x = i % width
	mov [cureent_y], eax  ; y = i / width

    popad
    ret

;-------------------------------------
;   Print new line
;-------------------------------------
print_nl:
	pushad
	
	mov DWORD [cureent_x], 0
	mov eax, [cureent_y]
	inc eax
	mov [cureent_y], eax

	; EAX contains [current_y]
	; i = x + y * width
	mov ecx, VIDEO_COLS * 2
	mul ecx
	add eax, VIDEO_MEMORY

	mov [cureent_char], eax
	
	popad
	ret

;-------------------------------------
;   Clear the screen
;-------------------------------------
clear_screen:
	pushad

	mov ebx, VIDEO_MEMORY
	mov al, 0
    mov ah, CHAR_ATTRIBUTE
	
	clear_screen_loop:
	cmp ebx, VIDEO_MEMORY_LIMIT
	je clear_screen_done
	mov [ebx], eax
	add ebx, 2
	jmp clear_screen_loop
	
	clear_screen_done:
	mov DWORD [cureent_char], VIDEO_MEMORY
	mov DWORD [cureent_x], 0
	mov DWORD [cureent_y], 0

	popad
	ret

;-------------------------------
;   Print hex value in EDX
;-------------------------------
print_reg_hex:
	pushad

	xor ecx, ecx  ; set to zero

	print_reg_hex_loop:
	
	cmp ecx, 8
	je print_reg_hex_end
	
	mov eax, edx
	and eax, 0xF
	add al, 0x30
	cmp al, 0x39
	jle print_reg_hex_continue
	
	add al, 7
	
	print_reg_hex_continue:
	
	mov ebx, print_hex_mem + 7
	sub ebx, ecx
	mov [ebx], al
	
	ror edx, 4
	inc ecx
	
	jmp print_reg_hex_loop

	print_reg_hex_end:
	
	mov ebx, print_hex_mem
	call print

	popad
	ret

;---------------------------------
;   Print integer value in EDX
;---------------------------------
print_reg_integer:
	pushad

	mov eax, edx
	xor ecx, ecx
	
	print_reg_integer_loop:
	
	cmp eax, 0
	je print_reg_integer_end
	
	mov ebx, 10
	xor edx, edx ; set to zero, needed for division
	div ebx
	
	add dl, 0x30  ; reminder of division
	mov ebx, print_integer_mem + 9
	sub ebx, ecx
	mov [ebx], dl
	inc ecx
	
	jmp print_reg_integer_loop
	
	print_reg_integer_end:
	
	mov ebx, print_integer_mem + 10
	sub ebx, ecx
	call print
	
	popad
	ret
	
;---------------------------------
;   Print binary value in EDX
;---------------------------------
print_reg_binary:
	pushad

	xor ecx, ecx  ; set to zero

	print_reg_binary_loop:
	
	cmp ecx, 32
	je print_reg_binary_end
	
	mov eax, edx
	and eax, 1
	add al, 0x30
	
	mov ebx, print_binary_mem + 31
	sub ebx, ecx
	mov [ebx], al
	
	ror edx, 1
	inc ecx
	
	jmp print_reg_binary_loop

	print_reg_binary_end:
	
	mov ebx, print_binary_mem
	call print
	
	popad
	ret


%endif ; print_32_included