
#include <kernel/gdt.h>
#include <libc/memory.h>

#define GDT_ENTRIES 5

static GDT_Descriptor_t GDT_Table[GDT_ENTRIES];

static GDT_Register_t GDT_Register;

GDT_Descriptor_t* GDT_get_descriptor(uint16_t index)
{
	if (index >= GDT_ENTRIES) { return NULL; }
	
	return &GDT_Table[index];
}

void GDT_set_descriptor(uint16_t index, uint32_t base_address, uint32_t limit, uint8_t access, uint8_t granuality)
{
	if (index >= GDT_ENTRIES) { return; }
	
	memset(&GDT_Table[index], 0, sizeof(GDT_Descriptor_t));
	
	// set limit and base addresses
	GDT_Table[index].BaseLow	= (uint16_t)(base_address & 0xFFFF);
	GDT_Table[index].BaseMid	= (uint8_t)((base_address >> 16) & 0xFF);
	GDT_Table[index].BaseHigh	= (uint8_t)((base_address >> 24) & 0xFF);
	
	GDT_Table[index].Access = access;
	GDT_Table[index].Limit = (uint16_t)(limit & 0xFFFF);
	
	GDT_Table[index].Granuality = (uint8_t)((limit >> 16) & 0x0F);
	GDT_Table[index].Granuality |= granuality & 0xF0;
}

void GDT_init()
{
	// set up GDTR register
	GDT_Register.size = (sizeof(GDT_Descriptor_t) * GDT_ENTRIES) - 1;
	GDT_Register.address = (uint32_t)&GDT_Table[0];

	// set null descriptor
	GDT_set_descriptor(0, 0, 0, 0, 0);

	// set default code descriptor
	GDT_set_descriptor
	(
		1, 0, 0xFFFFFFFF,
		GDT_DESC_READWRITE | GDT_DESC_EXEC_CODE | GDT_DESC_CODEDATA | GDT_DESC_MEMORY,
		GDT_GRAND_4K | GDT_GRAND_32BIT | GDT_GRAND_LIMITHI_MASK
	);

	// set default data descriptor
	GDT_set_descriptor
	(
		2, 0, 0xFFFFFFFF,
		GDT_DESC_READWRITE | GDT_DESC_CODEDATA | GDT_DESC_MEMORY,
		GDT_GRAND_4K | GDT_GRAND_32BIT | GDT_GRAND_LIMITHI_MASK
	);
}
