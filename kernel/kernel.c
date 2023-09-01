
#include <kernel/idt.h>
#include <drivers/screen.h>
#include <libc/stdef.h>


void main()
{
	IDT_init(0x8);
	clear_screen();
	print("hello world\n");
	print("AMK-OS started");
	__asm__ __volatile__("int $100");
}

