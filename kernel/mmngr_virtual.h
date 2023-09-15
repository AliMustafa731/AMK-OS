#ifndef mmngr_virtual_included
#define mmngr_virtual_included

#include <libc/stdint.h>

//------------------------------------------
//   Virtual Memory Manager
//------------------------------------------

// Page Entry bit flags
#define PAGE_PRESENT     0x1
#define PAGE_WRITABLE    0x2
#define PAGE_USER        0x4

extern void VMMngr_init();

extern int VMMngr_map_page(uintptr_t physical, uintptr_t virtual);

extern int VMMngr_map_pages(uintptr_t physical, uintptr_t virtual, int32_t length);

extern void Paging_enable();

extern void Paging_disable();

#endif // mmngr_virtual_included
