;---------------------------------
;    16-bit Real Mode
;---------------------------------
[org 0x7C00]
[bits 16]

%define KERNEL_OFFSET 0x1000

main_16:
	mov [drive_number], dl

	mov ax, 0x9000
	mov ss, ax
	mov bp, 0xFFFF
	mov sp, bp
	
	; loading the kernel
	mov ax, 0
	mov es, ax
	mov bx, KERNEL_OFFSET

	mov al, 20    ; sectors to read
	mov cl, 0x2   ; starting sector
	mov ch, 0x0   ; cylinder number
	mov dh, 0x0   ; head number
	;mov dl, 0x80
	call disk_load_16

	switch_to_pm:
    cli
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

    mov ebp, 0xFFFFFF
    mov esp, ebp

	call Enable_A20
	
	jmp KERNEL_OFFSET
	
	hlt
	jmp $


drive_number: db 0

%include "bootloader/print_16.inc"
%include "bootloader/disk_16.inc"
%include "bootloader/gdt_32.inc"
%include "bootloader/A20.inc"

times 510 - ($-$$) db 0
dw 0xAA55
