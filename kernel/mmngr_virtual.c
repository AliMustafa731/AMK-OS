
#include <kernel/mmngr_virtual.h>
#include <kernel/mmngr_physical.h>
#include <libc/string.h>

// current directory table
PageDirectory_t* VMMngr_cur_directory = NULL;

// current page directory base register
uint32_t VMMngr_PDBR = NULL;

inline void MMngr_load_PDBR(void* address)
{
    __asm__("movl %%eax, %%cr3" : : "a" (*address));
}

int VMMngr_switch_page_directory(PageDirectory_t* dir, uint32_t _PDBR)
{
    if (dir == NULL) { return 0; }

    VMMngr_cur_directory = dir;
    VMMngr_PDBR = _PDBR;

    MMngr_load_PDBR(VMMngr_PDBR);

    return 1;
}

int VMMngr_alloc_page(PageEntry_t *e)
{
    void* p = MMngr_alloc_block();
    if (p == NULL) { return 0; }

    PTE_set_frame(e, (uint32_t)p);
    PTE_set_flag(e, PTE_PRESENT | PTE_WRITABLE);

    return 1;
}

void VMMngr_free_page(PageEntry_t *e)
{
    void* p = (void*)PTE_get_frame(e);
    if (p != NULL) { MMngr_free_block(p); }

    PTE_unset_flag(e, PTE_PRESENT);
}

int VMMngr_map_page(void* physical, void* virtual)
{
    // get page directory
    PageDirectory_t* dir = VMMngr_get_page_directory();

    // get page table
    PageEntry_t* table_entry = &dir->entries[PAGE_DIRECTORY_INDEX((uint32_t)virtual)];

    if (!(table_entry & PTE_PRESENT)) // page table not present, allocate it
    {
        PageTable_t* table = (PageTable_t*)MMngr_alloc_block(); // allocates 4KB
        if (table == NULL) { return 0; }

        memset(table, 0, sizeof(PageTable_t));

        PTE_set_frame(table_entry, (uint32_t)table);
        PTE_set_flag(table_entry, PTE_PRESENT | PTE_WRITABLE);
    }

    // get page table
    PageTable_t* table = (PageTable_t*)PTE_get_frame(table_entry);

    // get page
    PageEntry_t* page = &table->entries[PAGE_TABLE_INDEX((uint32_t)virtual)];

    PTE_set_frame(page, (uint32_t)physical);
    PTE_set_flag(page, PTE_PRESENT | PTE_WRITABLE);

    return 1;
}
