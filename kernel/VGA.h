#ifndef vga_included
#define vga_included

#include <libc/stdint.h>

extern void update_cursor(int x, int y);

extern void print(char* text);

extern void print_nl();

extern void clear_screen();

#endif  // vga_included
