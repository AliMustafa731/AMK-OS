[org 0x7C00]
[bits 16]
	
	mov ax, 0x9000
	mov ss, ax
	mov bp, 0xFFFF
	mov sp, bp
	
	; loading 64 sectors (32KB) from disk
	mov bx, 0x7E00
	mov dh, 64
	call disk_load_16
	
	mov bx, msg_16bit_mode
	call print_16
	
	jmp switch_to_pm

%include "print_16.inc"
%include "disk_16.inc"


times 510 - ($-$$) db 0
dw 0xAA55

;-----------------------------------------------
;    End of sector 1 & start of protected mode
;-----------------------------------------------

%include "gdt_32.inc"
%include "print_32.inc"

[bits 16]

switch_to_pm:
    cli		; disable interrupts
    lgdt	[gdt_descriptor] ; load the GDT descriptor
    mov		eax, cr0
    or		eax, 0x1 ; set bit 1 in cr0
    mov		cr0, eax
    jmp		CODE_SEG:init_pm ; far jump

[bits 32]

init_pm:
    mov ax, DATA_SEG ;  update the segment registers
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000 ; update the stack right at the top of the free space
    mov esp, ebp

	;----------------------------------
	;   Enable Line A20
	;----------------------------------

	call	ctrl_wait_input_empty
	mov		al, 0xAD  ; disable keyboard command
	out		0x64, al

	call	ctrl_wait_input_empty
	mov		al, 0xD0  ; read output port command
	out		0x64, al

	call	ctrl_wait_output_full
	in		al, 0x60
	push	eax

	call	ctrl_wait_input_empty
	mov		al, 0xD1  ; write output port command
	out		0x64, al

	call	ctrl_wait_input_empty
	pop		eax
	or		al, 2  ; set bit 1 (eanble A20)
	out		0x60, al

	call	ctrl_wait_input_empty
	mov		al, 0xAE  ; enable keyboard command
	out		0x64, al
	call	ctrl_wait_input_empty

	jmp start_pm

ctrl_wait_input_empty:
    in		al, 0x64
    test	al, 2
    jnz		ctrl_wait_input_empty
	ret
ctrl_wait_output_full:
    in		al, 0x64
    test	al, 1
    jz		ctrl_wait_output_full
	ret


start_pm:
	call clear_screen
	
	mov ebx, msg_32bit_mode
	call print
	call print_nl
	
	mov ebx, msg_hello
	call print
	call print_nl
	call print
	call print_nl
	
	mov esi, 8
	mov edx, 0x7FFF
	call print_hex
	call print_nl
	call print_integer
	call print_nl
	call print_binary
	call print_nl
	
	jmp $

msg_hello: db "Hello World !!", 0
msg_16bit_mode: db "Started in 16-bit real mode", 0
msg_32bit_mode: db "Loaded 32-bit protected mode", 0


times 2048 db 0x00

