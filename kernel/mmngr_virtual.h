#ifndef mmngr_virtual_included
#define mmngr_virtual_included

#include <kernel/paging.h>
#include <libc/stdint.h>

#define PAGES_PER_TABLE      1024
#define PAGE_TABLES_PER_DIR  1024

#define PAGE_DIRECTORY_INDEX(x)  (((x) >> 22) & 0x3FF)
#define PAGE_TABLE_INDEX(x)      (((x) >> 12) & 0x3FF)
#define PAGE_OFFSET(x)           ((x) & 0xFFF)

typedef struct _PageDirectory_t
{
    PageEntry_t entries[PAGE_TABLES_PER_DIR];

} PageDirectory_t;

typedef struct _PageTable_t
{
    PageEntry_t entries[PAGES_PER_TABLE];

} PageTable_t;


extern void VMMngr_init();

extern int VMMngr_alloc_page(PageEntry_t *e);

extern void VMMngr_free_page(PageEntry_t *e);

extern int VMMngr_map_page(void* physical, void* virtual);

extern int VMMngr_switch_page_directory(PageDirectory_t* dir, uint32_t _PDBR);

extern PageDirectory_t* VMMngr_get_page_directory();



#endif // mmngr_virtual_included
