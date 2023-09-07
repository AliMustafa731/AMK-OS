%ifndef memorey_included
%define memorey_included

;-------------------------
;   Memory map entry
;-------------------------
struc memory_map_entry
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

;------------------------------------------------
;   Get memory map from BIOS
;   /in ES:DI -> destination buffer for entries
;   /ret BP = entry count
;------------------------------------------------
BIOS_get_memory_map:
    pushad
    xor   ebx, ebx
    xor   bp, bp           ; number of entries stored here
    mov   edx, 'PAMS'      ; 'SMAP'
    mov   eax, 0xE820
    mov   ecx, 24          ; memory map entry struct is 24 bytes
    int   0x15             ; get first entry
    jc    BIOS_mmap_error    
    cmp   eax, 'PAMS'        ; BIOS returns SMAP in eax
    jne   BIOS_mmap_error
    test  ebx, ebx           ; if ebx=0 then list is one entry long; bail out
    je    BIOS_mmap_error
    jmp   BIOS_mmap_start

BIOS_mmap_next_entry:
    mov    edx, 'PAMS'    ; some BIOS's trash this register
    mov    ecx, 24        ; entry is 24 bytes
    mov    eax, 0xE820
    int    0x15           ; get next entry

BIOS_mmap_start:
    jcxz   BIOS_mmap_skip_entry    ; if actual returned bytes is 0, skip entry
BIOS_mmap_notext:
    mov    ecx, [es:di + memory_map_entry.length]  ; get length (low dword)
    test   ecx, ecx                                ; if length is 0 skip it
    jne    short BIOS_mmap_good_entry
    mov    ecx, [es:di + memory_map_entry.length + 4]  ; get length (upper dword)
    jecxz  BIOS_mmap_skip_entry                        ; if length is 0 skip it

BIOS_mmap_good_entry:
    inc    bp              ; increment entry count
    add    di, 24          ; point di to next entry in buffer

BIOS_mmap_skip_entry:
    cmp    ebx, 0                 ; if ebx return is 0, list is done
    jne    BIOS_mmap_next_entry   ; get next entry
    jmp    BIOS_mmap_done

BIOS_mmap_error:
    stc

BIOS_mmap_done:
    popad
    ret

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
