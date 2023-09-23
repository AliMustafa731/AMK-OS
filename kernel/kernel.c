
#include <kernel/hardware/hardware.h>
#include <kernel/memory/mem_physical.h>
#include <kernel/memory/mem_virtual.h>
#include <kernel/hardware/PIT.h>
#include <kernel/graphics/VGA.h>
#include <kernel/bootinfo.h>

#include <libc/stdint.h>
#include <libc/string.h>
#include <kernel/user/cmd.h>


//------------------------------------
//   Kernel Main Entry
//   these parameters are passed
//   from the Bootloader
//------------------------------------
void kernel_main(Multiboot_info_t *boot_info, Memory_region_t *mem_regions)
{
    // initialize some hardware devices
    Hardware_init(boot_info, mem_regions);
    __asm__("sti");

    // start command line
    VGA_set_color(0x3F);

    CMD_start();
}
