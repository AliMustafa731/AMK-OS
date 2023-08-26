
#define VIDEO_MEMORY 0xB8000
#define VIDEO_MEMORY_LIMIT 0xB8FA0

void print(const char* text);
void clear_screen();

void main()
{
	clear_screen();
	print("hello world");
}

void clear_screen()
{
	char* c = (char*)VIDEO_MEMORY;
	while((int)c < VIDEO_MEMORY_LIMIT)
	{
		*c = 0;
		c += 2;
	}
}

void print(const char* text)
{
	char* c = text;
	char* mem = VIDEO_MEMORY;

	while(*c != 0)
	{
		*mem = *c;    mem++;
		*mem = 0x0E;  mem++;
		c++;
	}
}
