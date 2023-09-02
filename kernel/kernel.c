
#include <kernel/hardware.h>
#include <drivers/screen.h>
#include <libc/stdef.h>


void main()
{
	Hardware_init();
	clear_screen();
	print("hello world\n");
	print("AMK-OS started");
	__asm__ __volatile__("int $100");
	__asm__ __volatile__("sti");
}

