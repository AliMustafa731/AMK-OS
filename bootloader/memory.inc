%ifndef memorey_included
%define memorey_included

;-------------------------
;   Memory map entry
;-------------------------
struc MemoryMapEntry
    .base_address    resq    1
    .length          resq    1
    .type            resd    1
    .acpi_null       resd    1
endstruc

;----------------------------------
;   Multi-boot standard info
;----------------------------------
struc multiboot_info
    .flags             resd    1
    .memoryLo          resd    1
    .memoryHi          resd    1
    .bootDevice        resd    1
    .cmdLine           resd    1
    .mods_count        resd    1
    .mods_addr         resd    1
    .syms0             resd    1
    .syms1             resd    1
    .syms2             resd    1
    .mmap_length       resd    1
    .mmap_addr         resd    1
    .drives_length     resd    1
    .drives_addr       resd    1
    .config_table      resd    1
    .bootloader_name   resd    1
    .apm_table         resd    1
    .vbe_control_info  resd    1
    .vbe_mode_info     resd    1
    .vbe_mode          resw    1
    .vbe_interface_seg  resw    1
    .vbe_interface_off  resw    1
    .vbe_interface_len  resw    1
endstruc

;---------------------------------------------
;    Get memory size for > 64M configuations
;    Returns :
;      AX = KB between 1MB and 16MB
;      BX = number of 64K blocks above 16MB
;---------------------------------------------
BIOS_get_memory_size:
    push   ecx
    push   edx
    xor    ecx, ecx
    xor    edx, edx
    mov    ax, 0xE801
    int    0x15    
    jc     BIOS_mem_error
    cmp    ah, 0x86         ; unsupported function
    je     BIOS_mem_error
    cmp    ah, 0x80         ; invalid command
    je     BIOS_mem_error
    jcxz   BIOS_mem_use_ax  ; bios may have stored it in ax,bx or cx,dx. test if cx is 0
    mov    ax, cx           ; its not, so it should contain mem size; store it
    mov    bx, dx
BIOS_mem_use_ax:
    pop edx     ; AX and BX already contains mem size, return it
    pop ecx
    ret

BIOS_mem_error:
    pop edx
    pop ecx
    ret

%endif  ; memorey_included
