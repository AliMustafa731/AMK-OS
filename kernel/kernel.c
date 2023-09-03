
#include <kernel/hardware.h>
#include <kernel/pit.h>
#include <drivers/screen.h>
#include <libc/stdef.h>


void main()
{
    Hardware_init();
    PIT_start_counter(100, PIT_OCW_COUNTER_0, PIT_OCW_MODE_SQUAREWAVEGEN);

    clear_screen();
    print("hello world\n");
    print("AMK-OS started");
    __asm__("int $100");
    __asm__("sti");
}

