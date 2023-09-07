
#include <kernel/hardware.h>
#include <kernel/PIT.h>
#include <kernel/VGA.h>
#include <libc/stdint.h>
#include <libc/string.h>


void kernel_main()
{
    Hardware_init();
    PIT_start_counter(0xFFFF, PIT_OCW_COUNTER_0, PIT_OCW_MODE_SQUAREWAVEGEN);
    VGA_set_color(0x3F);
    
    clear_screen();
    print("hello world\n");
    print("AMK-OS started");
    asm("int $100");
    asm("sti");

    uint32_t val = 0x9999;
    printf("hello 0x%x hex = %i decimal, %s \n", val, val, "extra string");
}

