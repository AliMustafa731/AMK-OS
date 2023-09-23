#ifndef vga_included
#define vga_included

#include <libc/stdint.h>

#define VIDEO_MEMORY 0xB8000
#define VIDEO_MEMORY_LIMIT 0xB8FA0
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

extern int VGA_current_x;
extern int VGA_current_y;
extern int VGA_current_mem;
extern uint8_t VGA_color;

extern void VGA_update_position(int linear);

extern void update_cursor(int x, int y);

extern void print(uint8_t* text);

extern void printf(uint8_t* text, ...);

extern void print_nl();

extern void clear_screen();

extern void VGA_set_color(uint8_t color);

#endif  // vga_included
