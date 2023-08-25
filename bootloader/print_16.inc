%ifndef print_16_included
%define print_16_included

[bits 16]

;----------------------------------------
;   print text located at address BX
;----------------------------------------
print_16:
	pusha
	mov ah, 0x0E
	print_16_loop:
	mov al, [bx]
	cmp al, 0
	je print_16_done
	
	int 0x10
	inc bx
	
	jmp print_16_loop
	
print_16_done:
	popa
	ret

;----------------------------
;   print new line
;----------------------------
print_16_nl:
    pusha
    
    mov ah, 0x0e
    mov al, 0x0a ; newline char
    int 0x10
    mov al, 0x0d ; carriage return
    int 0x10
    
    popa
    ret

;----------------------------------------
;   print hex value stored in DX
;----------------------------------------
hex_out: db "0x0000", 0 ; reserve memory for our new string

print_16_hex:
	pusha
	mov cx, 0
	
print_16_hex_loop:
	cmp cx, 4
	je print_16_hex_end
	
	mov ax, dx
	and ax, 0x000F
	add al, 0x30
	cmp al, 0x39
	jle print_16_step_2
	
	add al, 7

print_16_step_2:
	mov bx, hex_out+5
	sub bx, cx
	mov [bx], al
	
	ror dx, 4
	inc cx
    jmp print_16_hex_loop

print_16_hex_end:
	mov bx, hex_out
	call print_16
	
	popa
	ret


%endif ; print_16_included