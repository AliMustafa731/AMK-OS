
#include <kernel/paging.h>


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

uint32_t PTE_get_frame(PageEntry_t e)
{
    return e & PTE_FRAME;
}
