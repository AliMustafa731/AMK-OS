;---------------------------------
;    16-bit Real Mode
;---------------------------------
[org 0x7C00]
[bits 16]

main_16:
	mov ax, 0x9000
	mov ss, ax
	mov bp, 0xFFFF
	mov sp, bp
	
	; loading 20 sectors (10KB) from disk
	mov bx, 0x7E00
	mov dh, 20
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
%include "A20.inc"

[bits 16]

switch_to_pm:
    cli		; disable interrupts
    lgdt	[gdt_descriptor] ; load the GDT descriptor
    mov		eax, cr0
    or		eax, 0x1 ; set bit 1 in cr0
    mov		cr0, eax
    jmp		CODE_SEG:start_pm ; far jump

[bits 32]

start_pm:
    mov ax, DATA_SEG ;  update the segment registers
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000
    mov esp, ebp

	call Enable_A20

	call clear_screen
	
	mov ebx, msg_32bit_mode
	call print
	call print_nl
	
	mov ebx, msg_hello
	call print
	call print_nl
	call print
	call print_nl
	
	mov edx, 0x7FFF
	call print_reg_hex
	call print_nl
	call print_reg_integer
	call print_nl
	call print_reg_binary
	call print_nl

	mov ecx, 0
	
	hlt
	jmp $

msg_hello: db "Hello World !!", 0
msg_16bit_mode: db "Started in 16-bit real mode", 0
msg_32bit_mode: db "Loaded 32-bit protected mode", 0


times 2048 db 0x00

