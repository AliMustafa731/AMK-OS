#ifndef paging_included
#define paging_included

#include <libc/stdint.h>

// Page Directory Entry bit flags
#define PDE_PRESENT     0x1    //0000000000000000000000000000001
#define PDE_WRITABLE    0x2    //0000000000000000000000000000010
#define PDE_USER        0x4    //0000000000000000000000000000100
#define PDE_PWT         0x8    //0000000000000000000000000001000
#define PDE_PCD         0x10   //0000000000000000000000000010000
#define PDE_ACCESSED    0x20   //0000000000000000000000000100000
#define PDE_DIRTY       0x40   //0000000000000000000000001000000
#define PDE_4MB         0x80   //0000000000000000000000010000000
#define PDE_CPU_GLOBAL  0x100  //0000000000000000000000100000000
#define PDE_LV4_GLOBAL  0x200  //0000000000000000000001000000000
#define PDE_FRAME       0xFFFFF000  //1111111111111111111000000000000

// Page Table Entry bit flags
#define PTE_PRESENT        0x1    //0000000000000000000000000000001
#define PTE_WRITABLE       0x2    //0000000000000000000000000000010
#define PTE_USER           0x4    //0000000000000000000000000000100
#define PTE_WRITETHOUGH    0x8    //0000000000000000000000000001000
#define PTE_NOT_CACHEABLE  0x10   //0000000000000000000000000010000
#define PTE_ACCESSED       0x20   //0000000000000000000000000100000
#define PTE_DIRTY          0x40   //0000000000000000000000001000000
#define PTE_PAT            0x80   //0000000000000000000000010000000
#define PTE_CPU_GLOBAL     0x100  //0000000000000000000000100000000
#define PTE_LV4_GLOBAL     0x200  //0000000000000000000001000000000
#define PTE_FRAME          0xFFFFF000  //1111111111111111111000000000000

typedef uint32_t PageEntry_t;

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

extern void Paging_enable();

extern void Paging_disable();

extern int Is_Paging();

extern void PTE_set_flag(PageEntry_t *e, uint32_t flags);

extern void PTE_unset_flag(PageEntry_t *e, uint32_t flags);

// set frame addres of the page entry
extern void PTE_set_frame(PageEntry_t *e, uint32_t address);

extern uint32_t PTE_get_frame(PageEntry_t *e);

#endif // paging_included
