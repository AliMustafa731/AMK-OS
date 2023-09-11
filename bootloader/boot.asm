;---------------------------------
;    16-bit Real Mode
;---------------------------------
[org 0x7C00]
[bits 16]

%define KERNEL_OFFSET  0x10000
%define MEMORY_MAP_LOC 0x9100
%define BOOT_INFO_LOC  0x9000

jmp main_16

%include "bootloader/disk_16.inc"
%include "bootloader/memory.inc"

main_16:
    mov [BOOT_INFO_LOC + multiboot_info.bootDevice], dl

    ;------------------------------------
    ;   set the stack at safe memorey
    ;------------------------------------
    mov ax, 0x8000
    mov ss, ax
    mov bp, 0xFFFF
    mov sp, bp

    ;------------------------------------------------
    ;   loading extra 4 sectors of the bootloader
    ;------------------------------------------------
    mov ax, 0x7E0
    mov es, ax
    mov bx, 0

    mov al, 4     ; sectors to read
    mov cl, 0x2   ; starting sector
    mov ch, 0x0   ; cylinder number
    mov dh, 0x0   ; head number
    mov dl, [BOOT_INFO_LOC + multiboot_info.bootDevice]
    call disk_load_16

    ;-----------------------------------------
    ;   Get memory map & size and Enable A20
    ;-----------------------------------------
    call Enable_A20

    xor eax, eax
    xor ebx, ebx
    call BIOS_get_memory_size

    mov word [BOOT_INFO_LOC + multiboot_info.memoryHi], bx
    mov word [BOOT_INFO_LOC + multiboot_info.memoryLo], ax

    xor eax, eax
    mov es, ax
    mov di, MEMORY_MAP_LOC  ; the destination buffer to store the memory map
    call BIOS_get_memory_map

    ;-----------------------------------
    ;   loading the kernel at 0x10000
    ;-----------------------------------
    mov ax, 0x1000
    mov es, ax
    mov bx, 0

    mov al, 32    ; sectors to read
    mov cl, 0x5   ; starting sector
    mov ch, 0x0   ; cylinder number
    mov dh, 0x0   ; head number
    mov dl, [BOOT_INFO_LOC + multiboot_info.bootDevice]
    call disk_load_16

    ;-------------------------------------
    ;   switch to 32-bit protected mode
    ;-------------------------------------
    switch_to_pm:
    cli
    lgdt   [gdt_descriptor] ; load the GDT descriptor
    mov    eax, cr0
    or     eax, 0x1 ; set bit 1 in cr0
    mov    cr0, eax
    jmp    CODE_SEG:start_pm ; far jump

[bits 32]

start_pm:
    mov ax, DATA_SEG ;  update the segment registers
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x1000000
    mov esp, ebp
    
    ;-----------------------------------------
    ;  give control to the C Kernel
    ;-----------------------------------------
    ;  push pointers to boot_info on the stack
    ;  so that kernel_main() can read it
    ;-----------------------------------------
    push MEMORY_MAP_LOC
    push BOOT_INFO_LOC
    jmp KERNEL_OFFSET
    
    hlt
    jmp $


times 510 - ($-$$) db 0
dw 0xAA55
;---------------------------------
;   End of sector 1
;---------------------------------

%include "bootloader/print_16.inc"
%include "bootloader/gdt_32.inc"
%include "bootloader/A20.inc"
