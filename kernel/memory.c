
#include <kernel/memory.h>
#include <libc/string.h>

#define MMNGR_BLOCK_SIZE 4096
#define MMNGR_BLOCKS_PER_BYTE 8


static int MMngr_memory_size = 0;  // size of physical memory in KB
static int MMngr_used_blocks = 0;
static int MMngr_max_blocks = 0;

// memory map bit array. Each bit represents a 4KB memory block
// (1 = used block), (0 = free block)
static uint32_t* MMngr_memory_map = (uint32_t*)0x100000;


// utilities
static inline int mmap_is_set(uint32_t bit)
{
    return MMngr_memory_map[bit / 32] & (1 << (bit % 32));
}

static inline void mmap_set(uint32_t bit)
{
    if (!mmap_is_set(bit))
    {
        MMngr_memory_map[bit / 32] |= (1 << (bit % 32));
        MMngr_used_blocks++;
    }
}

static inline void mmap_unset(uint32_t bit)
{
    if (mmap_is_set(bit))
    {
        MMngr_memory_map[bit / 32] &= ~(1 << (bit % 32));
        MMngr_used_blocks--;
    }
}

void MMngr_init(size_t mem_size)
{
    MMngr_memory_size = mem_size;
    MMngr_max_blocks  = (MMngr_memory_size * 1024) / MMNGR_BLOCK_SIZE;

    // By default, all memory is in use
    MMngr_used_blocks = MMngr_max_blocks;
    memset(MMngr_memory_map, 0xFF, MMngr_max_blocks / MMNGR_BLOCKS_PER_BYTE);
}

// accessors
int MMngr_get_memory_size() { return MMngr_memory_size; }
int MMngr_get_used_blocks() { return MMngr_used_blocks; }
int MMngr_get_free_blocks() { return MMngr_max_blocks - MMngr_used_blocks; }
int MMngr_get_max_blocks()  { return MMngr_max_blocks; }

void MMngr_enable_region(uint32_t base, size_t size)
{
    base /= MMNGR_BLOCK_SIZE;
    int blocks = size / MMNGR_BLOCK_SIZE;

    while (blocks >= 0)
    {
        mmap_unset(base);
        base++;
        blocks--;
    }

    // first block is always set. This insures allocs can't be 0 (NULL)
    mmap_set(0);
}

void MMngr_disable_region(uint32_t base, size_t size)
{
    base /= MMNGR_BLOCK_SIZE;
    int blocks = size / MMNGR_BLOCK_SIZE;

    while (blocks >= 0)
    {
        mmap_set(base);
        base++;
        blocks--;
    }
}

int MMngr_get_free_block_index()
{
    for (int i = 0 ; i < MMngr_max_blocks / 32 ; i++)
    {
        if (MMngr_memory_map[i] != 0xFFFFFFFF) // ignore if all bits are 1 (used blocks)
        {
            for (int j = 0 ; j < 32 ; j++) // test each bit
            {
                int bit = 1 << j;
                if ((MMngr_memory_map[i] & bit) == 0) // check for 0 (unused block)
                {
                    return j + i * 32;
                }
            }
        }
    }

    return -1;
}

void* MMngr_alloc_block()
{
    if (MMngr_get_free_blocks() < 1) { return NULL; }  // out of memory

    int index = MMngr_get_free_block_index();
    if (index == -1) { return NULL; }  // out of memory

    mmap_set(index);

    return (void*)(index * MMNGR_BLOCK_SIZE);
}

void MMngr_free_block(void* ptr)
{
    uint32_t index = ((uint32_t)ptr) / MMNGR_BLOCK_SIZE;
    mmap_unset(index);
}
