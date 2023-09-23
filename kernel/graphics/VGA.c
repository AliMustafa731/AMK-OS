
#include <kernel/graphics/VGA.h>
#include <kernel/hardware/hardware.h>
#include <libc/string.h>


int VGA_current_x = 0;
int VGA_current_y = 0;
int VGA_current_mem = 0;
uint8_t VGA_color = 0x0E;

void Check_NEW_LINE()
{
    VGA_current_y = (VGA_current_mem / 2) / VGA_WIDTH;
    
    if (VGA_current_y >= VGA_HEIGHT)
    {
        VGA_current_y = VGA_HEIGHT - 1;

        memcpy((void*)(VIDEO_MEMORY), (void*)(VIDEO_MEMORY + (VGA_WIDTH * 2)), (VGA_WIDTH * (VGA_HEIGHT - 1)) * 2);
    }
}

void VGA_set_color(uint8_t color)
{
    VGA_color = color;
}

void clear_screen()
{
    uint8_t* mem = (uint8_t*)VIDEO_MEMORY;

    while((int)mem < VIDEO_MEMORY_LIMIT)
    {
        *mem = 0x00;       mem++;
        *mem = VGA_color;  mem++;
    }

    VGA_current_mem = 0;
    VGA_current_x = 0;
    VGA_current_y = 0;

    update_cursor(VGA_current_x, VGA_current_y);
}

void print(uint8_t* text)
{
    uint8_t* c = text;
    uint8_t* mem = (uint8_t*)VIDEO_MEMORY;

    while(*c != 0)
    {
        if (*c == '\n')
        {
            print_nl();
        }
        else
        {
            mem[VGA_current_mem] = *c;         VGA_current_mem++;
            mem[VGA_current_mem] = VGA_color;  VGA_current_mem++;
        }
        c++;
    }

    VGA_current_x = (VGA_current_mem / 2) % VGA_WIDTH;
    VGA_current_y = (VGA_current_mem / 2) / VGA_WIDTH;

    update_cursor(VGA_current_x, VGA_current_y);
}

void printf(uint8_t* text, ...)
{
    uint8_t* c = text;
    uint8_t* mem = (uint8_t*)VIDEO_MEMORY;

    uint8_t *args = (uint8_t*)&text + sizeof(text);

    while(*c != 0)
    {
        if (*c == '\n')
        {
            print_nl();
        }
        else if (*c == '%')
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
            mem[VGA_current_mem] = *c;         VGA_current_mem++;
            mem[VGA_current_mem] = VGA_color;  VGA_current_mem++;
        }
        c++;
    }

    VGA_current_x = (VGA_current_mem / 2) % VGA_WIDTH;
    VGA_current_y = (VGA_current_mem / 2) / VGA_WIDTH;

    update_cursor(VGA_current_x, VGA_current_y);
}

void print_nl()
{
    VGA_current_x = 0;
    VGA_current_y += 1;
    VGA_current_mem = VGA_current_y * VGA_WIDTH * 2;

    Check_NEW_LINE();

    update_cursor(VGA_current_x, VGA_current_y);
}

void update_cursor(int x, int y)
{
    uint16_t pos = x + y * VGA_WIDTH;

    // 0x3D4 VGA index register port
    // 0x3D5 VGA data register port
    port_byte_out(0x3D4, 0xE); // 0x0E cursor position high byte
    port_byte_out(0x3D5, HIGH_BYTE(pos));
    port_byte_out(0x3D4, 0xF); // 0x0F cursor position low byte
    port_byte_out(0x3D5, LOW_BYTE(pos));
}

void VGA_update_position(int linear)
{
    VGA_current_mem = linear;
    VGA_current_x = (VGA_current_mem / 2) % VGA_WIDTH;
    VGA_current_y = (VGA_current_mem / 2) / VGA_WIDTH;

    update_cursor(VGA_current_x, VGA_current_y);
}
