
#include <kernel/hardware/hardware.h>
#include <kernel/memory/mem_physical.h>
#include <kernel/memory/mem_virtual.h>
#include <kernel/hardware/PIT.h>
#include <kernel/graphics/VGA.h>
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

    // print stuff
    VGA_set_color(0x3F);
    clear_screen();

    // memory size in KB
    uint32_t mem_size = 1024 + boot_info->m_memoryLo + boot_info->m_memoryHi * 64;
    printf("Total memory MB : %i\n", mem_size / 1024);

    // print the memory map
    for (int i = 0 ; i < 15 ; i++)
    {
        if ( i != 0 && mem_regions[i].startLo == 0)
        {
            break;
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
