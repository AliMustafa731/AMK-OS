
#include <kernel/exception.h>
#include <kernel/hardware.h>
#include <kernel/idt.h>
#include <drivers/screen.h>

//------------------------------------
//  exception handlers
//-----------------------------------
void Exception_divide_by_zero (uint32_t cs, uint32_t eip, uint32_t eflags);
void Exception_single_step_trap (uint32_t cs, uint32_t eip, uint32_t eflags);
void Exception_nmi_trap (uint32_t cs, uint32_t eip, uint32_t eflags);
void Exception_breakpoint_trap (uint32_t cs, uint32_t eip, uint32_t eflags);
void Exception_overflow_trap (uint32_t cs, uint32_t eip, uint32_t eflags);
void Exception_bounds_check (uint32_t cs, uint32_t eip, uint32_t eflags);
void Exception_invalid_opcode (uint32_t cs, uint32_t eip, uint32_t eflags);
void Exception_no_device (uint32_t cs, uint32_t eip, uint32_t eflags);
void Exception_double_abort (uint32_t cs, uint32_t err,uint32_t eip, uint32_t eflags);
void Exception_invalid_tss (uint32_t cs, uint32_t err,uint32_t eip, uint32_t eflags);
void Exception_no_segment (uint32_t cs, uint32_t err,uint32_t eip, uint32_t eflags);
void Exception_stack (uint32_t cs, uint32_t err, uint32_t eip, uint32_t eflags);
void Exception_general_protection (uint32_t cs, uint32_t err, uint32_t eip, uint32_t eflags);
void Exception_page (uint32_t cs, uint32_t err, uint32_t eip, uint32_t eflags);
void Exception_fpu (uint32_t cs, uint32_t eip, uint32_t eflags);
void Exception_alignment_check (uint32_t cs, uint32_t err,uint32_t eip, uint32_t eflags);
void Exception_machine_check_abort (uint32_t cs, uint32_t eip, uint32_t eflags);
void Exception_simd_fpu (uint32_t cs, uint32_t eip, uint32_t eflags);

void Exceptions_init()
{
    // install exceptions handelers
    uint16_t segment = 0x8;
    uint8_t flags = IDT_DESC_32_BIT | IDT_DESC_SEG_PRESENT;
    IDT_install_int(0  ,(uint32_t)Exception_divide_by_zero, segment, flags);
	IDT_install_int(1  ,(uint32_t)Exception_single_step_trap, segment, flags);
	IDT_install_int(2  ,(uint32_t)Exception_nmi_trap, segment, flags);
	IDT_install_int(3  ,(uint32_t)Exception_breakpoint_trap, segment, flags);
	IDT_install_int(4  ,(uint32_t)Exception_overflow_trap, segment, flags);
	IDT_install_int(5  ,(uint32_t)Exception_bounds_check, segment, flags);
	IDT_install_int(6  ,(uint32_t)Exception_invalid_opcode, segment, flags);
	IDT_install_int(7  ,(uint32_t)Exception_no_device, segment, flags);
	IDT_install_int(8  ,(uint32_t)Exception_double_abort, segment, flags);
	IDT_install_int(10 ,(uint32_t)Exception_invalid_tss, segment, flags);
	IDT_install_int(11 ,(uint32_t)Exception_no_segment, segment, flags);
	IDT_install_int(12 ,(uint32_t)Exception_stack, segment, flags);
	IDT_install_int(13 ,(uint32_t)Exception_general_protection, segment, flags);
	IDT_install_int(14 ,(uint32_t)Exception_page, segment, flags);
	IDT_install_int(16 ,(uint32_t)Exception_fpu, segment, flags);
	IDT_install_int(17 ,(uint32_t)Exception_alignment_check, segment, flags);
	IDT_install_int(18 ,(uint32_t)Exception_machine_check_abort, segment, flags);
	IDT_install_int(19 ,(uint32_t)Exception_simd_fpu, segment, flags);
}

//---------------------------------------
//  exception handlers implementation
//  currently, they just print a meesage
//  and halt the system.
//  I'll implement them later.
//---------------------------------------
void Exception_divide_by_zero (uint32_t cs, uint32_t eip, uint32_t eflags)
{
    __asm__("pusha");

    print("Division by zero\n");
    while(1){}  // halt

    __asm__("popa");
    __asm__("leave");
    __asm__("iret");
}

void Exception_single_step_trap (uint32_t cs, uint32_t eip, uint32_t eflags)
{
    __asm__("pusha");

    print("Single Step Trap\n");
    while(1){}  // halt

    __asm__("popa");
    __asm__("leave");
    __asm__("iret");
}

void Exception_nmi_trap (uint32_t cs, uint32_t eip, uint32_t eflags)
{
    __asm__("pusha");

    print("Non Maskable Trap fault\n");
    while(1){}  // halt

    __asm__("popa");
    __asm__("leave");
    __asm__("iret");
}

void Exception_breakpoint_trap (uint32_t cs, uint32_t eip, uint32_t eflags)
{
    __asm__("pusha");

    print("Breakpoint Trap fault\n");
    while(1){}  // halt

    __asm__("popa");
    __asm__("leave");
    __asm__("iret");
}

void Exception_overflow_trap (uint32_t cs, uint32_t eip, uint32_t eflags)
{
    __asm__("pusha");

    print("Overflow Trap fault\n");
    while(1){}  // halt

    __asm__("popa");
    __asm__("leave");
    __asm__("iret");
}

void Exception_bounds_check (uint32_t cs, uint32_t eip, uint32_t eflags)
{
    __asm__("pusha");

    print("Bounds Check fault\n");
    while(1){}  // halt

    __asm__("popa");
    __asm__("leave");
    __asm__("iret");
}

void Exception_invalid_opcode (uint32_t cs, uint32_t eip, uint32_t eflags)
{
    __asm__("pusha");

    print("Invalid Opcode fault\n");
    while(1){}  // halt

    __asm__("popa");
    __asm__("leave");
    __asm__("iret");
}

void Exception_no_device (uint32_t cs, uint32_t eip, uint32_t eflags)
{
    __asm__("pusha");

    print("No Device fault\n");
    while(1){}  // halt

    __asm__("popa");
    __asm__("leave");
    __asm__("iret");
}

void Exception_double_abort (uint32_t cs, uint32_t err, uint32_t eip, uint32_t eflags)
{
    __asm__("pusha");

    print("Double Abort fault\n");
    while(1){}  // halt

    __asm__("popa");
    __asm__("leave");
    __asm__("iret");
}

void Exception_invalid_tss (uint32_t cs, uint32_t err, uint32_t eip, uint32_t eflags)
{
    __asm__("pusha");

    print("Invalid TSS fault\n");
    while(1){}  // halt

    __asm__("popa");
    __asm__("leave");
    __asm__("iret");
}

void Exception_no_segment (uint32_t cs, uint32_t err, uint32_t eip, uint32_t eflags)
{
    __asm__("pusha");

    print("No Segment fault\n");
    while(1){}  // halt

    __asm__("popa");
    __asm__("leave");
    __asm__("iret");
}

void Exception_stack (uint32_t cs, uint32_t err, uint32_t eip, uint32_t eflags)
{
    __asm__("pusha");

    print("Stack fault\n");
    while(1){}  // halt

    __asm__("popa");
    __asm__("leave");
    __asm__("iret");
}

void Exception_general_protection (uint32_t cs, uint32_t err, uint32_t eip, uint32_t eflags)
{
    __asm__("pusha");

    print("General Protection fault\n");
    while(1){}  // halt

    __asm__("popa");
    __asm__("leave");
    __asm__("iret");
}

void Exception_page (uint32_t cs, uint32_t err, uint32_t eip, uint32_t eflags)
{
    __asm__("pusha");

    print("Paging fualt\n");
    while(1){}  // halt

    __asm__("popa");
    __asm__("leave");
    __asm__("iret");
}

void Exception_fpu (uint32_t cs, uint32_t eip, uint32_t eflags)
{
    __asm__("pusha");

    print("FPU fault\n");
    while(1){}  // halt

    __asm__("popa");
    __asm__("leave");
    __asm__("iret");
}

void Exception_alignment_check (uint32_t cs, uint32_t err, uint32_t eip, uint32_t eflags)
{
    __asm__("pusha");

    print("Alignment Check fault\n");
    while(1){}  // halt

    __asm__("popa");
    __asm__("leave");
    __asm__("iret");
}

void Exception_machine_check_abort (uint32_t cs, uint32_t eip, uint32_t eflags)
{
    __asm__("pusha");

    print("Machine Check Abort fault\n");
    while(1){}  // halt

    __asm__("popa");
    __asm__("leave");
    __asm__("iret");
}

void Exception_simd_fpu (uint32_t cs, uint32_t eip, uint32_t eflags)
{
    __asm__("pusha");

    print("SIMD FPU fault\n");
    while(1){}  // halt

    __asm__("popa");
    __asm__("leave");
    __asm__("iret");
}

