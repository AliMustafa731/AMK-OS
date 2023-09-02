#ifndef hardware_included
#define hardware_included

#include <libc/stdef.h>

extern void Hardware_init();

extern void PIC_init();    // Programable Interrupt Controller

extern void Timer_init();  // Programable Interval Timer

// Accessing the mapped I/O ports
extern uint8_t  port_byte_in(uint16_t port);
extern void     port_byte_out(uint16_t port, uint8_t data);
extern uint16_t port_word_in(uint16_t port);
extern void     port_word_out(uint16_t port, uint16_t data);

#endif  // hardware_included
