
#include <kernel/hardware.h>
#include <kernel/IDT.h>
#include <kernel/GDT.h>
#include <kernel/PIC.h>
#include <kernel/PIT.h>
#include <kernel/exception.h>

void Hardware_init()
{
    GDT_init();
    IDT_init(0x8);
    PIC_init(32, 40);
    PIT_init();
    Exceptions_init();
}

inline void Hardware_interrupt_done(uint8_t irq_num)
{
    if (irq_num > 15) { return; }

    //! test if we need to send end-of-interrupt to second PIC
    if (irq_num > 7) { PIC_send_command(PIC_OCW2_MASK_EOI, 1); }

    //! always send end-of-interrupt to primary PIC
    PIC_send_command(PIC_OCW2_MASK_EOI, 0);
}

//------------------------------------
//  Accessing the mapped I/O ports
//------------------------------------
uint8_t port_byte_in(uint16_t port)
{
    uint8_t result;

    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));

    return result;
}

void port_byte_out(uint16_t port, uint8_t data)
{
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

uint16_t port_word_in(uint16_t port)
{
    uint16_t result;

    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));

    return result;
}

void port_word_out(uint16_t port, uint16_t data)
{
    __asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}
