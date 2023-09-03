
#include <kernel/idt.h>
#include <libc/memory.h>
#include <drivers/screen.h>

static IDT_Descriptor_t IDT_Table[IDT_MAX_INTERRUPTS];

static IDT_Register_t IDT_Register;

//---------------------------------------------
//  default handler
//  these parameters are pushed
//  to the stack by the processor
//---------------------------------------------
void IDT_Default_Handler(uint32_t cs, uint32_t eip, uint32_t eflags)
{
    __asm__("pusha");

    print("\nAn Interrupt was called\n");

    __asm__("popa");
    __asm__("leave");
    __asm__("iret");
}

IDT_Descriptor_t* IDT_get_int(uint16_t index)
{
    if (index >= IDT_MAX_INTERRUPTS) { return NULL; }
    
    return &IDT_Table[index];
}

void IDT_install_int(uint16_t index, uint32_t handler_address, uint16_t segment, uint8_t flags)
{
    if (index >= IDT_MAX_INTERRUPTS) { return; }
    
    memset((uint8_t*)&IDT_Table[index], 0, sizeof(IDT_Descriptor_t));
    
    IDT_Table[index].BaseLow   =  (uint16_t)(handler_address & 0xFFFF);
    IDT_Table[index].BaseHigh  =  (uint16_t)((handler_address >> 16) & 0xFFFF);
    IDT_Table[index].Reserved  =  0;
    IDT_Table[index].Flags     =  flags;
    IDT_Table[index].Segment   =  segment;
}

void IDT_init(uint16_t segment)
{
    // set up IDTR register
    IDT_Register.size = (sizeof(IDT_Descriptor_t) * IDT_MAX_INTERRUPTS) - 1;
    IDT_Register.address = (uint32_t)&IDT_Table[0];
    
    // register default handlers
    for (int i = 0 ; i < IDT_MAX_INTERRUPTS ; i++)
    {
        IDT_install_int(i, (uint32_t)IDT_Default_Handler, segment, IDT_DESC_32_BIT | IDT_DESC_SEG_PRESENT);
    }
    
    __asm__ __volatile__("lidtl (%0)" : : "r" (&IDT_Register));
}

