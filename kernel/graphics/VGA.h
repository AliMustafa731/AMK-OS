#ifndef vga_included
#define vga_included

#include <libc/stdint.h>

#define VIDEO_MEMORY 0xB8000
#define VIDEO_MEMORY_LIMIT 0xB8FA0

#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_SIZE (VGA_WIDTH * VGA_HEIGHT)

#define VGA_WIDTH_BYTES  (VGA_WIDTH * 2)
#define VGA_HEIGHT_BYTES (VGA_HEIGHT * 2)
#define VGA_SIZE_BYTES   (VGA_WIDTH_BYTES * VGA_HEIGHT_BYTES)

extern size_t VGA_current_x;
extern size_t VGA_current_y;
extern size_t VGA_current_mem;
extern uint8_t VGA_color;

extern void VGA_update_position(size_t linear);

extern void VGA_update_position_xy(size_t x, size_t y);

extern void VGA_update_cursor();

extern void VGA_set_color(uint8_t color);

extern void print_char(uint8_t c);

extern void print(uint8_t* text);

extern void printf(uint8_t* text, ...);

extern void print_nl();  // print "new line"

extern void clear_screen();

#endif  // vga_included
