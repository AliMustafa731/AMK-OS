#ifndef hardware_included
#define hardware_included

#include <libc/stdint.h>
#include <kernel/bootinfo.h>

extern void Hardware_init(Multiboot_info_t *boot_info, Memory_region_t *mem_regions);

extern void Hardware_interrupt_done(uint8_t irq_num);

// Accessing the mapped I/O ports
extern uint8_t  port_byte_in(uint16_t port);
extern void     port_byte_out(uint16_t port, uint8_t data);
extern uint16_t port_word_in(uint16_t port);
extern void     port_word_out(uint16_t port, uint16_t data);

#endif  // hardware_included
