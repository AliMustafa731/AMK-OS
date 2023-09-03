[bits 32]
[extern PIT_interrupt_handler]
[global ASM_PIT_interrupt_handler]

ASM_PIT_interrupt_handler:
    pushad

    call PIT_interrupt_handler  ; call the C handler

    popad
    iret
