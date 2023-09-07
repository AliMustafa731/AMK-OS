
#include <kernel/hardware.h>
#include <kernel/PIT.h>
#include <kernel/VGA.h>
#include <libc/stdint.h>
#include <libc/string.h>

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

void kernel_main(multiboot_info *boot_info)
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
    printf("Drive number : 0x%x, Memory %i MB\n", boot_info->m_bootDevice, (boot_info->m_memoryHi * 64) / 1024);
}

