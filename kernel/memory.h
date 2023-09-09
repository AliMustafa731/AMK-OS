#ifndef memory_included
#define memory_included

#include <libc/stdint.h>

extern void MMngr_init(size_t mem_size);

extern void MMngr_enable_region(uint32_t base, size_t size);

extern void MMngr_disable_region(uint32_t base, size_t size);

extern int MMngr_get_free_block_index();

extern void* MMngr_alloc_block();

extern void MMngr_free_block(void* ptr);

// accessors
extern int MMngr_get_memory_size();
extern int MMngr_get_used_blocks();
extern int MMngr_get_free_blocks();
extern int MMngr_get_max_blocks();

// Structures for Info passed from the BIOS
typedef struct _Memory_region_t
{
    uint32_t  startLo;
    uint32_t  startHi;
    uint32_t  sizeLo;
    uint32_t  sizeHi;
    uint32_t  type;
    uint32_t  acpi_3_0;
} __attribute__((packed)) Memory_region_t;

typedef struct _Multiboot_info_t
{
    uint32_t  m_flags;
    uint32_t  m_memoryLo;
    uint32_t  m_memoryHi;
    uint32_t  m_bootDevice;
    uint32_t  m_cmdLine;
    uint32_t  m_modsCount;
    uint32_t  m_modsAddr;
    uint32_t  m_syms0;
    uint32_t  m_syms1;
    uint32_t  m_syms2;
    uint32_t  m_mmap_length;
    uint32_t  m_mmap_addr;
    uint32_t  m_drives_length;
    uint32_t  m_drives_addr;
    uint32_t  m_config_table;
    uint32_t  m_bootloader_name;
    uint32_t  m_apm_table;
    uint32_t  m_vbe_control_info;
    uint32_t  m_vbe_mode_info;
    uint16_t  m_vbe_mode;
    uint32_t  m_vbe_interface_addr;
    uint16_t  m_vbe_interface_len;
} __attribute__((packed)) Multiboot_info_t;

#endif  // memory_included
