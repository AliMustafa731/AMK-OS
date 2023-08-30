
#include <libc/stdef.h>
#include <drivers/screen.h>


void main()
{
	IDT_init(0x8);
	__asm__ __volatile__("int $2");
	clear_screen();
	print("hello world\n");
	print("AMK-OS started");
}

