#ifndef screen_included
#define screen_included

#include <libc/stdef.h>

extern void update_cursor(int x, int y);

extern void print(char* text);

extern void print_nl();

extern void clear_screen();

#endif  // screen_included
