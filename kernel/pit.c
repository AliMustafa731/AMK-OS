
#include <kernel/pit.h>
#include <kernel/pic.h>
#include <kernel/idt.h>
#include <drivers/screen.h>

//-----------------------------------------------
//	Programmable Interval Timer
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
    IDT_install_int(32, (uint32_t)PIT_interrupt_handler, 0x8, IDT_DESC_32_BIT | IDT_DESC_SEG_PRESENT);
}

void PIT_start_counter(uint16_t freq, uint8_t counter, uint8_t mode)
{
    if (freq == 0) { return; }

	uint16_t divisor = (uint16_t)(1193181 / (uint16_t)freq);

	//! send operational command
	uint8_t ocw = 0;
	ocw = (ocw & ~PIT_OCW_MASK_MODE) | mode;
	ocw = (ocw & ~PIT_OCW_MASK_RL) | PIT_OCW_RL_DATA;
	ocw = (ocw & ~PIT_OCW_MASK_COUNTER) | counter;
	port_byte_out(PIT_REG_COMMAND, ocw);

	//! set frequency rate
	port_byte_out(PIT_REG_COUNTER0, divisor & 0xFF);
	port_byte_out(PIT_REG_COUNTER0, (divisor >> 8) & 0xFF);

	//! reset tick count
	PIT_ticks = 0;
}
