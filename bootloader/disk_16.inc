%ifndef disk_16_included
%define disk_16_included

[bits 16]

;----------------------------------------------------
;   load sectors into ES:BX
;   AL : number of sectors to read
;   CL : starting sector (start from index 1)
;   CH : cylinder number (start from index 0)
;   DH : head number (start from index 0)
;   DL : drive number
;----------------------------------------------------
disk_load_16:
    pusha
    push bp
    mov bp, sp

    ; store number of sectors to read on stack
    sub sp, 2
    mov BYTE [bp - 2], al

    ; call BIOS interrupt 13
    disk_16_loop:
    push ax
    push cx
    push dx

    mov ah, 0x02
    int 0x13
    jc disk_16_retry
    cmp al, BYTE [bp - 2]
    jne disk_16_retry

    jmp disk_16_end

    disk_16_retry:
    pop ax
    pop cx
    pop dx
    jmp disk_16_loop

    disk_16_end:
    pop ax
    pop cx
    pop dx

    add sp, 2
    pop bp
    popa
    ret


%endif ; disk_16_included