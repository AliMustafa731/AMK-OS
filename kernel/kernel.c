
#include <kernel/hardware.h>
#include <kernel/mmngr_physical.h>
#include <kernel/mmngr_virtual.h>
#include <kernel/PIT.h>
#include <kernel/VGA.h>
#include <kernel/bootinfo.h>
#include <libc/stdint.h>
#include <libc/string.h>


char* str_memory_types[] =
{
    "Available",        //memory_region.type == 1
    "Reserved",         //memory_region.type == 2
    "ACPI Reclaim",     //memory_region.type == 3
    "ACPI NVS Memory"   //memory_region.type == 4
};

// these parameters are passed from the Bootloader
void kernel_main(Multiboot_info_t *boot_info, Memory_region_t *mem_regions)
{
    Hardware_init();

    uint32_t mem_size = 1024 + boot_info->m_memoryLo + boot_info->m_memoryHi * 64;
    MMngr_init(mem_size);
    VMMngr_init();

    PIT_start_counter(0xFFFF, PIT_OCW_COUNTER_0, PIT_OCW_MODE_SQUAREWAVEGEN);

    VGA_set_color(0x3F);
    clear_screen();
    print("hello world\n");
    print("AMK-OS started\n");
    asm("sti");

    printf("Drive number : 0x%x, Memory %i MB\n", boot_info->m_bootDevice, mem_size / 1024);
    print("Physical Memory map :\n");

    for (int i = 0 ; i < 15 ; i++)
    {
        if (mem_regions[i].type > 4) // if type is > 4 mark it reserved
        {
            mem_regions[i].type = 2;
        }
        
        // if start address is 0, there is no more entries, break out
        if (i > 0 && mem_regions[i].startLo == 0) { break; }

        if (mem_regions[i].type == 1) // enable available regions
        {
            MMngr_enable_region(mem_regions[i].startLo, mem_regions[i].sizeLo);
        }

        printf ("region %i: start: 0x%x%x length (bytes): 0x%x%x type: %i (%s)\n", i, 
        mem_regions[i].startHi, mem_regions[i].startLo,
        mem_regions[i].sizeHi,  mem_regions[i].sizeLo,
        mem_regions[i].type,    str_memory_types[mem_regions[i].type-1]);
    }

    print("\nCurrent Memory Blocks : 4KB each\n");
    printf("Total : %i, Used : %i, Free : %i\n",
            MMngr_get_max_blocks(), MMngr_get_used_blocks(), MMngr_get_free_blocks()
    );
}

