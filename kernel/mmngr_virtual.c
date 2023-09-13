
#include <kernel/mmngr_virtual.h>
#include <kernel/mmngr_physical.h>
#include <libc/string.h>

//------------------------------------------
//   Virtual Memory Manager
//------------------------------------------

// current directory table
PageDirectory_t* VMMngr_cur_directory = NULL;

// current page directory base register
uintptr_t VMMngr_PDBR = NULL;

void MMngr_load_PDBR(uintptr_t address)
{
    __asm__("movl %%eax, %%cr3" : : "a" (address));
}

void VMMngr_init()
{
    PageDirectory_t* dir = (PageDirectory_t*)MMngr_alloc_block();

    // identity map the first 4MB of memory
    VMMngr_map_pages(0, 0, 0x400000);

    VMMngr_switch_page_directory(dir);
}

int VMMngr_switch_page_directory(PageDirectory_t* dir)
{
    if (dir == NULL) { return 0; }

    VMMngr_cur_directory = dir;
    VMMngr_PDBR = (uintptr_t)&dir->entries[0];

    MMngr_load_PDBR(VMMngr_PDBR);

    return 1;
}

PageDirectory_t* VMMngr_get_page_directory() { return VMMngr_cur_directory; }

int VMMngr_alloc_page(PageEntry_t *e)
{
    void* p = MMngr_alloc_block();
    if (p == NULL) { return 0; }

    PTE_set_frame(e, (uintptr_t)p);
    PTE_set_flag(e, PTE_PRESENT | PTE_WRITABLE);

    return 1;
}

void VMMngr_free_page(PageEntry_t *e)
{
    void* p = (void*)PTE_get_frame(e);
    if (p != NULL) { MMngr_free_block(p); }

    PTE_unset_flag(e, PTE_PRESENT | PTE_WRITABLE);
}

int VMMngr_map_page(uintptr_t physical, uintptr_t virtual)
{
    // get page directory
    PageDirectory_t* dir = VMMngr_get_page_directory();

    // get page table
    PageEntry_t* table_entry = &dir->entries[PAGE_DIRECTORY_INDEX(virtual)];

    if (!(*table_entry & PTE_PRESENT)) // page table not present, allocate it
    {
        if (VMMngr_alloc_page(table_entry) == 0) // allocate 4KB block
        {
            return 0;
        } 
        memset((void*)PTE_get_frame(table_entry), 0, sizeof(PageTable_t)); // clear
    }

    // get page table
    PageTable_t* table = (PageTable_t*)PTE_get_frame(table_entry);

    // get page
    PageEntry_t* page = &table->entries[PAGE_TABLE_INDEX(virtual)];

    PTE_set_frame(page, physical);
    PTE_set_flag(page, PTE_PRESENT | PTE_WRITABLE);

    return 1;
}

int VMMngr_map_pages(uintptr_t physical, uintptr_t virtual, int32_t length)
{
    while(length >= 0)
    {
        VMMngr_map_page(physical, virtual);
        physical += 4096;
        virtual += 4096;
        length -= 4096;
    }
}
