#ifndef pic_included
#define pic_included

#include <libc/stdef.h>

//------------------------------------------
//  Programable Interrupt Controller
//------------------------------------------

//-----------------------------------------------
//	Devices connected to the PICs. May be useful
//	when enabling and disabling irq's
//-----------------------------------------------

//! The following devices use PIC 1 to generate interrupts
#define	PIC_IRQ_TIMER			0
#define	PIC_IRQ_KEYBOARD		1
#define	PIC_IRQ_SERIAL2			3
#define	PIC_IRQ_SERIAL1			4
#define	PIC_IRQ_PARALLEL2		5
#define	PIC_IRQ_DISKETTE		6
#define	PIC_IRQ_PARALLEL1		7

//! The following devices use PIC 2 to generate interrupts
#define	PIC_IRQ_CMOSTIMER		0
#define	PIC_IRQ_CGARETRACE		1
#define	PIC_IRQ_AUXILIARY		4
#define	PIC_IRQ_FPU				5
#define	PIC_IRQ_HDC				6

//-----------------------------------------------
//	Command words are used to control the devices
//-----------------------------------------------

//! Command Word 2 bit masks. Use when sending commands
#define	PIC_OCW2_MASK_L1		1		//00000001
#define	PIC_OCW2_MASK_L2		2		//00000010
#define	PIC_OCW2_MASK_L3		4		//00000100
#define	PIC_OCW2_MASK_EOI		0x20	//00100000
#define	PIC_OCW2_MASK_SL		0x40	//01000000
#define	PIC_OCW2_MASK_ROTATE	0x80	//10000000

//! Command Word 3 bit masks. Use when sending commands
#define	PIC_OCW3_MASK_RIS		1		//00000001
#define	PIC_OCW3_MASK_RIR		2		//00000010
#define	PIC_OCW3_MASK_MODE		4		//00000100
#define	PIC_OCW3_MASK_SMM		0x20	//00100000
#define	PIC_OCW3_MASK_ESMM		0x40	//01000000
#define	PIC_OCW3_MASK_D7		0x80	//10000000

extern void PIC_init(uint8_t base_1, uint8_t base_2);

extern uint8_t PIC_read_data(uint8_t pic_num);

extern void PIC_send_data(uint8_t data, uint8_t pic_num);

extern void PIC_send_command(uint8_t command, uint8_t pic_num);

#endif  // pic_included
