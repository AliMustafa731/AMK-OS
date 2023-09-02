
#include <kernel/pic.h>
#include <kernel/hardware.h>

//-----------------------------------------------
//  Programable Interrupt Controller
//-----------------------------------------------
//-----------------------------------------------
//	Controller Registers
//-----------------------------------------------

//! PIC 1 register port addresses
#define PIC1_REG_COMMAND	0x20
#define PIC1_REG_STATUS		0x20
#define PIC1_REG_DATA		0x21
#define PIC1_REG_IMR		0x21

//! PIC 2 register port addresses
#define PIC2_REG_COMMAND	0xA0
#define PIC2_REG_STATUS		0xA0
#define PIC2_REG_DATA		0xA1
#define PIC2_REG_IMR		0xA1

//-----------------------------------------------
//	Initialization Command Bit Masks
//-----------------------------------------------

//! Initialization Control Word 1 bit masks
#define PIC_ICW1_MASK_IC4		0x1			//00000001
#define PIC_ICW1_MASK_SNGL		0x2			//00000010
#define PIC_ICW1_MASK_ADI		0x4			//00000100
#define PIC_ICW1_MASK_LTIM		0x8			//00001000
#define PIC_ICW1_MASK_INIT		0x10		//00010000

//! Initialization Control Words 2 and 3 do not require bit masks

//! Initialization Control Word 4 bit masks
#define PIC_ICW4_MASK_UPM		0x1			//00000001
#define PIC_ICW4_MASK_AEOI		0x2			//00000010
#define PIC_ICW4_MASK_MS		0x4			//00000100
#define PIC_ICW4_MASK_BUF		0x8			//00001000
#define PIC_ICW4_MASK_SFNM		0x10		//00010000

//-----------------------------------------------
//	Initialization Command 1 control bits
//-----------------------------------------------

#define PIC_ICW1_IC4_EXPECT				1			//1
#define PIC_ICW1_IC4_NO					0			//0
#define PIC_ICW1_SNGL_YES				2			//10
#define PIC_ICW1_SNGL_NO				0			//00
#define PIC_ICW1_ADI_CALLINTERVAL4		4			//100
#define PIC_ICW1_ADI_CALLINTERVAL8		0			//000
#define PIC_ICW1_LTIM_LEVELTRIGGERED	8			//1000
#define PIC_ICW1_LTIM_EDGETRIGGERED		0			//0000
#define PIC_ICW1_INIT_YES				0x10		//10000
#define PIC_ICW1_INIT_NO				0			//00000

//-----------------------------------------------
//	Initialization Command 4 control bits
//-----------------------------------------------

#define PIC_ICW4_UPM_86MODE			1			//1
#define PIC_ICW4_UPM_MCSMODE		0			//0
#define PIC_ICW4_AEOI_AUTOEOI		2			//10
#define PIC_ICW4_AEOI_NOAUTOEOI		0			//0
#define PIC_ICW4_MS_BUFFERMASTER	4			//100
#define PIC_ICW4_MS_BUFFERSLAVE		0			//0
#define PIC_ICW4_BUF_MODEYES		8			//1000
#define PIC_ICW4_BUF_MODENO			0			//0
#define PIC_ICW4_SFNM_NESTEDMODE	0x10		//10000
#define PIC_ICW4_SFNM_NOTNESTED		0			//0

inline uint8_t PIC_read_data(uint8_t pic_num)
{
    return port_byte_in((pic_num == 0) ? PIC1_REG_DATA : PIC2_REG_DATA);
}

inline void PIC_send_data(uint8_t data, uint8_t pic_num)
{
    port_byte_out((pic_num == 0) ? PIC1_REG_DATA : PIC2_REG_DATA, data);
}

inline void PIC_send_command(uint8_t command, uint8_t pic_num)
{
    port_byte_out((pic_num == 0) ? PIC1_REG_COMMAND : PIC2_REG_COMMAND, command);
}

void PIC_init(uint8_t base_1, uint8_t base_2)
{	
	// ICW1 initialization
	port_byte_out(PIC1_REG_COMMAND, 0x11);
	port_byte_out(PIC2_REG_COMMAND, 0x11);
	
    // ICW2
	port_byte_out(PIC1_REG_DATA, base_1);  // map (IRQ 0 to IRQ 7)
	port_byte_out(PIC2_REG_DATA, base_2);  // map (IRQ 8 to IRQ 15)
	
	// ICW3 for master PIC is the IR that connects to secondary pic in binary format
	// ICW3 for secondary PIC is the IR that connects to master pic in decimal format
	port_byte_out(PIC1_REG_DATA, 0x4);  // 0x04 => 0100, second bit (IR line 2)
	port_byte_out(PIC2_REG_DATA, 0x2);  // 0x02 => 0010, IR line 2
	
	// ICW4 Set x86 mode, bit 0 enables 80x86 mode
	port_byte_out(PIC1_REG_DATA, 0x1);
	port_byte_out(PIC2_REG_DATA, 0x1);
}
