%ifndef disk_16_included
%define disk_16_included

[bits 16]

;----------------------------------------------------
;   load 'DH' sectors from drive 'DL' into ES:BX
;----------------------------------------------------
disk_load_16:
	pusha
	
	disk_16_loop_retry:
	push dx
	
	disk_16_loop:
	mov ah, 0x02
	mov al, dh    ; number of sectors to read (0x01 .. 0x80)
	mov cl, 0x02  ; starting sector
	mov ch, 0x00  ; cylinder number
	push dx
	mov dh, 0x00  ; head number
	int 0x13
	pop dx
	jc disk_16_loop
	
	pop dx
	cmp al, dh
	jne sectors_16_error
	popa
	ret


sectors_16_error:
	push bx
    mov bx, SECTORS_ERROR
    call print_16
	pop bx
	
	jmp disk_16_loop_retry


SECTORS_ERROR: db "Incorrect number of sectors read", 0


%endif ; disk_16_included