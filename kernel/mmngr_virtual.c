
#include <kernel/mmngr_virtual.h>
#include <kernel/mmngr_physical.h>
#include <libc/string.h>

//------------------------------------------
//   Virtual Memory Manager
//------------------------------------------

// each page entry is 32 bit

#define PAGE_DIRECTORY_INDEX(x) (((x) >> 22) & 0x3FF)
#define PAGE_TABLE_INDEX(x)     (((x) >> 12) & 0x3FF)
#define PAGE_FRAME(x)           ((x) & 0xFFFFF000)

uint32_t* page_dir   = (uint32_t*)0;
uint32_t* page_table = (uint32_t*)0;

void VMMngr_init()
{
    // identity map the first 16MB
    page_dir = MMngr_alloc_block();

    for (int i = 0 ; i < 4 ; i++)
    {
        page_dir[i] = (uint32_t)MMngr_alloc_block();
        page_dir[i] |= PAGE_PRESENT | PAGE_WRITABLE;
        page_table = (uint32_t*)(PAGE_FRAME(page_dir[i]));

        for (int j = 0 ; j < 1024 ; j++)
        {
            page_table[j] = ((j + i * 1024) * 4096) | 3;
        }
    }
    
    __asm__("movl %%eax, %%cr3" : : "a" (page_dir));

    Paging_enable();
}

int VMMngr_map_page(uintptr_t physical, uintptr_t virtual)
{
    // get page table
    uint32_t* table_entry = &page_dir[PAGE_DIRECTORY_INDEX(virtual)];

    if ((*table_entry & PAGE_PRESENT) != PAGE_PRESENT) // page table not present, allocate it
    {
        void* p = MMngr_alloc_block();
        if (p == NULL) { return 0; }
        memset((void*)p, 0, 4096); // clear

        *table_entry |= ((uint32_t)p) | PAGE_PRESENT | PAGE_WRITABLE;
    }

    // get page table
    uint32_t* table = (uint32_t*)PAGE_FRAME(*table_entry);

    // get page
    uint32_t* page = &table[PAGE_TABLE_INDEX(virtual)];

    *page |= PAGE_FRAME(physical) | PAGE_PRESENT | PAGE_WRITABLE;

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

// other utilities
inline void Paging_enable()
{
    __asm__("mov %cr0, %eax");
    __asm__("or $0x80000001, %eax"); // set bit 31
    __asm__("mov %eax, %cr0");
}

inline void Paging_disable()
{
    __asm__("mov %cr0, %eax");
    __asm__("and $0x7FFFFFFF, %eax"); // clear bit 31
    __asm__("mov %eax, %cr0");
}
