
#include <kernel/hardware.h>
#include <kernel/PIT.h>
#include <kernel/VGA.h>
#include <libc/stdint.h>
#include <libc/string.h>

typedef struct _memory_region
{
	uint32_t	startLo;
	uint32_t	startHi;
	uint32_t	sizeLo;
	uint32_t	sizeHi;
	uint32_t	type;
	uint32_t	acpi_3_0;
} memory_region;

typedef struct _multiboot_info
{
	uint32_t	m_flags;
	uint32_t	m_memoryLo;
	uint32_t	m_memoryHi;
	uint32_t	m_bootDevice;
	uint32_t	m_cmdLine;
	uint32_t	m_modsCount;
	uint32_t	m_modsAddr;
	uint32_t	m_syms0;
	uint32_t	m_syms1;
	uint32_t	m_syms2;
	uint32_t	m_mmap_length;
	uint32_t	m_mmap_addr;
	uint32_t	m_drives_length;
	uint32_t	m_drives_addr;
	uint32_t	m_config_table;
	uint32_t	m_bootloader_name;
	uint32_t	m_apm_table;
	uint32_t	m_vbe_control_info;
	uint32_t	m_vbe_mode_info;
	uint16_t	m_vbe_mode;
	uint32_t	m_vbe_interface_addr;
	uint16_t	m_vbe_interface_len;
} multiboot_info;

char* str_memory_types[] =
{
	"Available",        //memory_region.type==0
	"Reserved",         //memory_region.type==1
	"ACPI Reclaim",     //memory_region.type==2
	"ACPI NVS Memory"   //memory_region.type==3
};

void kernel_main(multiboot_info *boot_info)
{
    Hardware_init();
    PIT_start_counter(0xFFFF, PIT_OCW_COUNTER_0, PIT_OCW_MODE_SQUAREWAVEGEN);
    VGA_set_color(0x3F);
    
    clear_screen();
    print("hello world\n");
    print("AMK-OS started\n");
    asm("sti");

    memory_region *mem_regions = (memory_region*)0x9000;
    uint32_t mem_size = 1024 + boot_info->m_memoryLo + boot_info->m_memoryHi * 64;

    printf("Drive number : 0x%x, Memory %i MB\n", boot_info->m_bootDevice, mem_size / 1024);

    for (int i = 0 ; i < 15 ; i++)
    {
        // if type is > 4 mark it reserved
        if (mem_regions[i].type > 4) { mem_regions[i].type = 1; }
        
        // if start address is 0, there is no more entries, break out
        if (i > 0 && mem_regions[i].startLo == 0) { break; }

        printf ("region %i: start: 0x%x%x length (bytes): 0x%x%x type: %i (%s)\n", i, 
        mem_regions[i].startHi, mem_regions[i].startLo,
        mem_regions[i].sizeHi,  mem_regions[i].sizeLo,
        mem_regions[i].type,    str_memory_types[mem_regions[i].type-1]);
	}
}

