#ifndef mmngr_physical_included
#define mmngr_physical_included

#include <libc/stdint.h>

extern void MMngr_init(size_t mem_size);

extern void MMngr_enable_region(uint32_t base, size_t size);

extern void MMngr_disable_region(uint32_t base, size_t size);

extern int MMngr_get_free_block_index();

extern void* MMngr_alloc_block();

extern void MMngr_free_block(void* ptr);

// accessors
extern int MMngr_get_memory_size();
extern int MMngr_get_used_blocks();
extern int MMngr_get_free_blocks();
extern int MMngr_get_max_blocks();

#endif  // mmngr_physical_included
