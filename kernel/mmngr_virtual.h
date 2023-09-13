#ifndef mmngr_virtual_included
#define mmngr_virtual_included

#include <kernel/paging.h>
#include <libc/stdint.h>

//------------------------------------------
//   Virtual Memory Manager
//------------------------------------------

extern void VMMngr_init();

extern int VMMngr_alloc_page(PageEntry_t *e);

extern void VMMngr_free_page(PageEntry_t *e);

extern int VMMngr_map_page(uintptr_t physical, uintptr_t virtual);

extern int VMMngr_map_pages(uintptr_t physical, uintptr_t virtual, int32_t length);

extern int VMMngr_switch_page_directory(PageDirectory_t* dir);

extern PageDirectory_t* VMMngr_get_page_directory();


#endif // mmngr_virtual_included
