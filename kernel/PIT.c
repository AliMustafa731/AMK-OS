
#include <kernel/PIT.h>
#include <kernel/PIC.h>
#include <kernel/cpu/IDT.h>
#include <kernel/VGA.h>

//-----------------------------------------------
//    Programmable Interval Timer
//-----------------------------------------------

static volatile uint32_t PIT_ticks = 0;

//---------------------------------------------
//  timer interrupt handler
//  these parameters are pushed
//  to the stack by the processor
//---------------------------------------------
void PIT_interrupt_handler(uint32_t cs, uint32_t eip, uint32_t eflags)
{
    __asm__("pusha");

    PIT_ticks++;
    Hardware_interrupt_done(PIC_IRQ_TIMER);

    __asm__("popa");
    __asm__("leave");
    __asm__("iret");
}

void PIT_init()
{
    IDT_install_int(PIC_INT_TIMER, (uintptr_t)PIT_interrupt_handler, 0x8, IDT_DESC_32_BIT | IDT_DESC_SEG_PRESENT);
}

void PIT_start_counter(uint16_t start_value, uint8_t counter, uint8_t mode)
{
    if (start_value == 0) { return; }

    //! send operational command
    uint8_t ocw = 0;
    ocw = (ocw & ~PIT_OCW_MASK_MODE) | mode;
    ocw = (ocw & ~PIT_OCW_MASK_RL) | PIT_OCW_RL_DATA;
    ocw = (ocw & ~PIT_OCW_MASK_COUNTER) | counter;
    port_byte_out(PIT_REG_COMMAND, ocw);

    //! set frequency rate
    port_byte_out(PIT_REG_COUNTER0, start_value & 0xFF);
    port_byte_out(PIT_REG_COUNTER0, (start_value >> 8) & 0xFF);

    //! reset tick count
    PIT_ticks = 0;
}
