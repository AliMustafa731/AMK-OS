[bits 32]
[extern IDT_Default_Handler]
[global ASM_IDT_Handler]

ASM_IDT_Handler:
	; 1. Save CPU state
	pushad
	mov ax, ds
	push eax ; save the data segment descriptor
	mov ax, 0x10  ; kernel data segment descriptor
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
    ; 2. Call C handler
	call IDT_Default_Handler
	
    ; 3. Restore state
	pop eax 
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	popad
	iret
