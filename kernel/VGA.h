#ifndef vga_included
#define vga_included

#include <libc/stdint.h>

extern void update_cursor(int x, int y);

extern void print(uint8_t* text);

extern void printf(uint8_t* text, ...);

extern void print_nl();

extern void clear_screen();

extern void VGA_set_color(uint8_t color);

#endif  // vga_included
