#ifndef screen_included
#define screen_included

#include <libc/stdef.h>

void update_cursor(int x, int y);

void print(char* text);

void print_nl();

void clear_screen();

#endif  // screen_included
