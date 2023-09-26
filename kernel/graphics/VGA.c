
#include <kernel/graphics/VGA.h>
#include <kernel/hardware/hardware.h>
#include <libc/string.h>

#define MAKE_WORD(x, y) (uint16_t)((x) << 8 | (y))

size_t VGA_current_x = 0;
size_t VGA_current_y = 0;
size_t VGA_current_mem = 0;
uint8_t VGA_color = 0x0E;

void VGA_set_color(uint8_t color)
{
    VGA_color = color;
}

void VGA_update_cursor()
{
    size_t pos = VGA_current_mem;
    // 0x3D4 VGA index register port
    // 0x3D5 VGA data register port
    port_byte_out(0x3D4, 0xE); // 0x0E cursor position high byte
    port_byte_out(0x3D5, HIGH_BYTE(pos));
    port_byte_out(0x3D4, 0xF); // 0x0F cursor position low byte
    port_byte_out(0x3D5, LOW_BYTE(pos));
}

void VGA_update_position(size_t linear)
{
    VGA_current_mem = linear;
    VGA_current_x = VGA_current_mem % VGA_WIDTH;
    VGA_current_y = VGA_current_mem / VGA_WIDTH;

    VGA_update_cursor();
}

void VGA_update_position_xy(size_t x, size_t y)
{
    VGA_current_x = x;
    VGA_current_y = y;
    VGA_current_mem = x + y * VGA_WIDTH;

    VGA_update_cursor();
}

void clear_screen()
{
    uint16_t* mem = (uint16_t*)VIDEO_MEMORY;

    for (int i = 0 ; i < VGA_WIDTH * VGA_HEIGHT ; i++)
    {
        *mem = MAKE_WORD(VGA_color, 0);
        mem++;
    }

    VGA_update_position(0);
}

void ScrollDown()
{
    // copy the whole buffer up by one row
    memcpy((void*)(VIDEO_MEMORY), (void*)(VIDEO_MEMORY + VGA_WIDTH_BYTES), VGA_SIZE_BYTES - VGA_WIDTH_BYTES);

    // empty last row
    uint16_t* mem = (uint16_t*)(VIDEO_MEMORY_LIMIT - VGA_WIDTH_BYTES);

    for (int i = 0 ; i < VGA_WIDTH ; i++)
    {
        *mem = MAKE_WORD(VGA_color, 0);
        mem++;
    }

    VGA_current_mem -= VGA_WIDTH;
    VGA_update_position(VGA_current_mem);
}

void print_char(uint8_t c)
{
    if (VGA_current_mem >= VGA_WIDTH * VGA_HEIGHT)
    {
        ScrollDown();
    }

    uint16_t* mem = (uint16_t*)VIDEO_MEMORY;

    if (c == '\n')
    {
        print_nl();
    }
    else if (c == '\b')
    {
        mem[--VGA_current_mem] = MAKE_WORD(VGA_color, 0);
        VGA_update_position(VGA_current_mem);
    }
    else
    {
        mem[VGA_current_mem++] = MAKE_WORD(VGA_color, c);
        VGA_update_position(VGA_current_mem);
    }
}

void print(uint8_t* text)
{
    uint8_t* c = text;

    while(*c != 0)
    {
        print_char(*c);
        c++;
    }
}

void printf(uint8_t* text, ...)
{
    uint8_t* c = text;

    uint8_t *args = (uint8_t*)&text + sizeof(text);

    while(*c != 0)
    {
        if (*c == '%')
        {
            c++;

            if (*c == 's')
            {
                uint8_t* str = *((uint8_t**)args);  args += sizeof(uint8_t*);

                print(str);
            }
            else if (*c == 'i')
            {
                uint32_t val = *((uint32_t*)args);  args += sizeof(uint32_t);

                uint8_t buff[15];
                int_to_string(val, buff);
                print(buff);
            }
            else if (*c == 'x')
            {
                uint32_t val = *((uint32_t*)args);  args += sizeof(uint32_t);
                
                uint8_t buff[15];
                hex_to_string(val, buff);
                print(buff);
            }
        }
        else
        {
            print_char(*c);
        }

        c++;
    }
}

// print "new line"
void print_nl()
{
    if (VGA_current_mem >= VGA_WIDTH * VGA_HEIGHT)
    {
        ScrollDown();
    }

    VGA_update_position_xy(0, VGA_current_y + 1);
}
