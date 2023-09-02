
#include <drivers/screen.h>
#include <kernel/hardware.h>

#define VIDEO_MEMORY 0xB8000
#define VIDEO_MEMORY_LIMIT 0xB8FA0
#define WIDTH 80
#define HEIGHT 25

int current_x = 0;
int current_y = 0;
int current_mem = 0;


void clear_screen()
{
	char* mem = (char*)VIDEO_MEMORY;

	while((int)mem < VIDEO_MEMORY_LIMIT)
	{
		*mem = 0x00;  mem++;
		*mem = 0x0E;  mem++;
	}

	current_mem = 0;
	current_x = 0;
	current_y = 0;

	update_cursor(current_x, current_y);
}

void print(char* text)
{
	char* c = text;
	char* mem = (char*)VIDEO_MEMORY;

	while(*c != 0)
	{
		if (*c == 0x0A)
		{
			print_nl();
		}
		else
		{
			mem[current_mem] = *c;    current_mem++;
			mem[current_mem] = 0x0E;  current_mem++;
		}
		c++;
	}

	current_x = (current_mem / 2) % WIDTH;
	current_y = (current_mem / 2) / WIDTH;

	update_cursor(current_x, current_y);
}

void print_nl()
{
	current_x = 0;
	current_y += 1;
	current_mem = current_y * WIDTH * 2;

	update_cursor(current_x, current_y);
}

void update_cursor(int x, int y)
{
	uint16_t pos = x + y * WIDTH;

	// 0x3D4 VGA index register port
	// 0x3D5 VGA data register port
	port_byte_out(0x3D4, 0xE); // 0x0E cursor position high byte
	port_byte_out(0x3D5, HIGH_BYTE(pos));
	port_byte_out(0x3D4, 0xF); // 0x0F cursor position low byte
	port_byte_out(0x3D5, LOW_BYTE(pos));
}
