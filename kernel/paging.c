
#include <kernel/paging.h>

void Paging_enable()
{
    __asm__("movl %cr0, %eax");
    __asm__("or $0x80000000, %eax"); // set bit 31
    __asm__("movl %eax, %cr0");
}

void Paging_disable()
{
    __asm__("movl %cr0, %eax");
    __asm__("and $0x7FFFFFFF, %eax"); // clear bit 31
    __asm__("movl %eax, %cr0");
}

int Is_Paging()
{
    int res;
    __asm__("movl %eax, %cr3");
    __asm__("and $0x7FFFFFFF, %%eax" : "=a" (res) : );
    return res;
}

void PTE_set_flag(PageEntry_t *e, uint32_t flags)
{
    *e |= flags;
}

void PTE_unset_flag(PageEntry_t *e, uint32_t flags)
{
    *e &= ~flags;
}

// set frame addres of the page entry
void PTE_set_frame(PageEntry_t *e, uint32_t address)
{
    *e = (*e & ~PTE_FRAME) | address;
}

uint32_t PTE_get_frame(PageEntry_t *e)
{
    return *e & PTE_FRAME;
}
