
#include <kernel/hardware.h>
#include <kernel/PIT.h>
#include <kernel/VGA.h>
#include <libc/stdint.h>
#include <libc/string.h>


void kernel_main()
{
    Hardware_init();
    PIT_start_counter(0xFFFF, PIT_OCW_COUNTER_0, PIT_OCW_MODE_SQUAREWAVEGEN);

    clear_screen();
    print("hello world\n");
    print("AMK-OS started");
    __asm__("int $100");
    __asm__("sti");
}

