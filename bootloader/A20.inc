%ifndef A20_included
%define A20_included

[bits 32]

;----------------------------------
;   Enable Line A20
;----------------------------------
Enable_A20:
    pushad

    call    ctrl_wait_input_empty
    mov        al, 0xAD  ; disable keyboard command
    out        0x64, al

    call    ctrl_wait_input_empty
    mov        al, 0xD0  ; read output port command
    out        0x64, al

    call    ctrl_wait_output_full
    in        al, 0x60
    push    eax

    call    ctrl_wait_input_empty
    mov        al, 0xD1  ; write output port command
    out        0x64, al

    call    ctrl_wait_input_empty
    pop        eax
    or        al, 2  ; set bit 1 (eanble A20)
    out        0x60, al

    call    ctrl_wait_input_empty
    mov        al, 0xAE  ; enable keyboard command
    out        0x64, al
    call    ctrl_wait_input_empty

    popad
    ret

ctrl_wait_input_empty:
    in        al, 0x64
    test    al, 2
    jnz        ctrl_wait_input_empty
    ret
ctrl_wait_output_full:
    in        al, 0x64
    test    al, 1
    jz        ctrl_wait_output_full
    ret

%endif ; A20_included