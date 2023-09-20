
#include <kernel/keyboard.h>
#include <kernel/hardware.h>
#include <kernel/IDT.h>
#include <kernel/PIC.h>

//==============================
//   Keyboard Interface
//==============================

// keyboard controller & encoder registers
#define KYBRD_CTRL_STATUS_REG   0x64
#define KYBRD_CTRL_CMD_REG      0x64
#define KYBRD_ENC_INPUT_BUFFER  0x60
#define KYBRD_ENC_CMD_REG       0x60

// keyboard controller status register's masks
#define KYBRD_CTRL_STATS_MASK_OUT_BUF   0x1     //00000001
#define KYBRD_CTRL_STATS_MASK_IN_BUF    0x2     //00000010
#define KYBRD_CTRL_STATS_MASK_SYSTEM    0x4     //00000100
#define KYBRD_CTRL_STATS_MASK_CMD_DATA  0x8     //00001000
#define KYBRD_CTRL_STATS_MASK_LOCKED    0x10    //00010000
#define KYBRD_CTRL_STATS_MASK_AUX_BUF   0x20    //00100000
#define KYBRD_CTRL_STATS_MASK_TIMEOUT   0x40    //01000000
#define KYBRD_CTRL_STATS_MASK_PARITY    0x80    //10000000

// keyboard controller commands
#define KYBRD_CTRL_CMD_READ              0x20
#define KYBRD_CTRL_CMD_WRITE             0x60
#define KYBRD_CTRL_CMD_SELF_TEST         0xAA
#define KYBRD_CTRL_CMD_INTERFACE_TEST    0xAB
#define KYBRD_CTRL_CMD_DISABLE           0xAD
#define KYBRD_CTRL_CMD_ENABLE            0xAE
#define KYBRD_CTRL_CMD_READ_IN_PORT      0xC0
#define KYBRD_CTRL_CMD_READ_OUT_PORT     0xD0
#define KYBRD_CTRL_CMD_WRITE_OUT_PORT     0xD1
#define KYBRD_CTRL_CMD_READ_TEST_INPUTS   0xE0
#define KYBRD_CTRL_CMD_SYSTEM_RESET       0xFE
#define KYBRD_CTRL_CMD_MOUSE_DISABLE     0xA7
#define KYBRD_CTRL_CMD_MOUSE_ENABLE      0xA8
#define KYBRD_CTRL_CMD_MOUSE_PORT_TEST   0xA9
#define KYBRD_CTRL_CMD_MOUSE_WRITE       0xD4

// keyboard encoder commands
#define KYBRD_ENC_CMD_SET_LED           0xED
#define KYBRD_ENC_CMD_ECHO              0xEE
#define KYBRD_ENC_CMD_SCAN_CODE_SET     0xF0
#define KYBRD_ENC_CMD_ID                0xF2
#define KYBRD_ENC_CMD_AUTODELAY         0xF3
#define KYBRD_ENC_CMD_ENABLE            0xF4
#define KYBRD_ENC_CMD_RESETWAIT         0xF5
#define KYBRD_ENC_CMD_RESETSCAN         0xF6
#define KYBRD_ENC_CMD_ALL_AUTO          0xF7
#define KYBRD_ENC_CMD_ALL_MAKEBREAK         0xF8
#define KYBRD_ENC_CMD_ALL_MAKEONLY          0xF9
#define KYBRD_ENC_CMD_ALL_MAKEBREAK_AUTO    0xFA
#define KYBRD_ENC_CMD_SINGLE_AUTOREPEAT     0xFB
#define KYBRD_ENC_CMD_SINGLE_MAKEBREAK      0xFC
#define KYBRD_ENC_CMD_SINGLE_BREAKONLY      0xFD
#define KYBRD_ENC_CMD_RESEND              0xFE
#define KYBRD_ENC_CMD_RESET               0xFF

// keyboard error codes
#define KYBRD_ERR_ID_RET             0x83AB
#define KYBRD_ERR_BUF_OVERRUN        0x0
#define KYBRD_ERR_BAT                0xAA    //note: can also be L. shift key make code
#define KYBRD_ERR_ECHO_RET           0xEE
#define KYBRD_ERR_ACK                0xFA
#define KYBRD_ERR_BAT_FAILED         0xFC
#define KYBRD_ERR_DIAG_FAILED        0xFD
#define KYBRD_ERR_RESEND_CMD         0xFE
#define KYBRD_ERR_KEY                0xFF

// interrupt handler
void keyboard_interrupt_handler()
{
    __asm__("pusha");
    __asm__("cli");  // disable hardware interrupts

    // DO SOMETHING
    Hardware_interrupt_done(PIC_IRQ_KEYBOARD);

    __asm__("sti");  // enable hardware interrupts
    __asm__("popa");
    __asm__("leave");
    __asm__("iret");
}

void Keyboard_init()
{
    // install interrupt handler
    IDT_install_int(PIC_INT_KEYBOARD, (uintptr_t)keyboard_interrupt_handler, 0x8, IDT_DESC_32_BIT | IDT_DESC_SEG_PRESENT);
}

//-----------------------
//   utilities
//-----------------------
inline uint8_t keyboard_ctrl_read_status()
{
    return port_byte_in(KYBRD_CTRL_STATUS_REG);
}

inline void keyboard_ctrl_send_command(uint8_t command)
{
    // wait for the input buffer to be empty
    while(keyboard_ctrl_read_status() & KYBRD_CTRL_STATS_MASK_IN_BUF){}

    port_byte_out(KYBRD_CTRL_CMD_REG, command);
}

inline void keyboard_enc_send_command(uint8_t command)
(
    // wait for the input buffer to be empty
    while(keyboard_ctrl_read_status() & KYBRD_CTRL_STATS_MASK_IN_BUF){}

    port_byte_out(KYBRD_ENC_CMD_REG, command);
)
