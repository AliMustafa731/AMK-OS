
#include <kernel/hardware/hardware.h>
#include <kernel/memory/mem_physical.h>
#include <kernel/memory/mem_virtual.h>
#include <kernel/cpu/IDT.h>
#include <kernel/cpu/GDT.h>
#include <kernel/hardware/PIC.h>
#include <kernel/hardware/PIT.h>
#include <kernel/cpu/exception.h>
#include <kernel/input/keyboard.h>

void Hardware_init(Multiboot_info_t *boot_info, Memory_region_t *mem_regions)
{
    GDT_init();
    IDT_init(0x8);
    PIC_init(32, 40);
    PIT_init();
    Exceptions_init();
    Keyboard_init();

    PIT_start_counter(0xFFFF, PIT_OCW_COUNTER_0, PIT_OCW_MODE_SQUAREWAVEGEN);

    // initialize memory map
    uint32_t mem_size = 1024 + boot_info->m_memoryLo + boot_info->m_memoryHi * 64;
    MMngr_init(mem_size);

    // print the memory map
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
    }

    // the first 4MB are reserved
    MMngr_disable_region(0, 0x400000);

    // enable paging
    VMMngr_init();
}

inline void Hardware_interrupt_done(uint8_t irq_num)
{
    if (irq_num > 15) { return; }

    //! test if we need to send end-of-interrupt to second PIC
    if (irq_num > 7) { PIC_send_command(PIC_OCW2_MASK_EOI, 1); }

    //! always send end-of-interrupt to primary PIC
    PIC_send_command(PIC_OCW2_MASK_EOI, 0);
}

//------------------------------------
//  Accessing the mapped I/O ports
//------------------------------------
uint8_t port_byte_in(uint16_t port)
{
    uint8_t result;

    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));

    return result;
}

void port_byte_out(uint16_t port, uint8_t data)
{
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

uint16_t port_word_in(uint16_t port)
{
    uint16_t result;

    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));

    return result;
}

void port_word_out(uint16_t port, uint16_t data)
{
    __asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}
