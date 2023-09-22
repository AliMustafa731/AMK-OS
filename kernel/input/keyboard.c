
#include <kernel/input/keyboard.h>
#include <kernel/hardware/hardware.h>
#include <kernel/cpu/IDT.h>
#include <kernel/hardware/PIC.h>
#include <kernel/graphics/VGA.h>
#include <libc/string.h>

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

//=============================
//   Scan Codes set 1
//   Index = Scancode
//   Value = Keycode
//=============================
static int keyboard_scancodes[] = {

    // key             scancode
    KEY_UNKNOWN,       //0
    KEY_ESCAPE,        //1
    KEY_1,             //2
    KEY_2,             //3
    KEY_3,             //4
    KEY_4,             //5
    KEY_5,             //6
    KEY_6,             //7
    KEY_7,             //8
    KEY_8,             //9
    KEY_9,             //0xa
    KEY_0,             //0xb
    KEY_MINUS,         //0xc
    KEY_EQUAL,         //0xd
    KEY_BACKSPACE,     //0xe
    KEY_TAB,           //0xf
    KEY_Q,             //0x10
    KEY_W,             //0x11
    KEY_E,             //0x12
    KEY_R,             //0x13
    KEY_T,             //0x14
    KEY_Y,             //0x15
    KEY_U,             //0x16
    KEY_I,             //0x17
    KEY_O,             //0x18
    KEY_P,             //0x19
    KEY_LEFTBRACKET,   //0x1a
    KEY_RIGHTBRACKET,  //0x1b
    KEY_KP_ENTER,      //0x1c
    KEY_LCTRL,         //0x1d
    KEY_A,             //0x1e
    KEY_S,             //0x1f
    KEY_D,             //0x20
    KEY_F,             //0x21
    KEY_G,             //0x22
    KEY_H,             //0x23
    KEY_J,             //0x24
    KEY_K,             //0x25
    KEY_L,             //0x26
    KEY_SEMICOLON,     //0x27
    KEY_QUOTE,         //0x28
    KEY_GRAVE,         //0x29
    KEY_LSHIFT,        //0x2a
    KEY_BACKSLASH,     //0x2b
    KEY_Z,             //0x2c
    KEY_X,             //0x2d
    KEY_C,             //0x2e
    KEY_V,             //0x2f
    KEY_B,             //0x30
    KEY_N,             //0x31
    KEY_M,             //0x32
    KEY_COMMA,         //0x33
    KEY_DOT,           //0x34
    KEY_SLASH,         //0x35
    KEY_RSHIFT,        //0x36
    KEY_KP_ASTERISK,   //0x37
    KEY_RALT,          //0x38
    KEY_SPACE,         //0x39
    KEY_CAPSLOCK,      //0x3a
    KEY_F1,            //0x3b
    KEY_F2,            //0x3c
    KEY_F3,            //0x3d
    KEY_F4,            //0x3e
    KEY_F5,            //0x3f
    KEY_F6,            //0x40
    KEY_F7,            //0x41
    KEY_F8,            //0x42
    KEY_F9,            //0x43
    KEY_F10,           //0x44
    KEY_KP_NUMLOCK,    //0x45
    KEY_SCROLLLOCK,    //0x46
    KEY_HOME,          //0x47
    KEY_UP,            //0x48    //keypad up arrow
    KEY_PAGEUP,        //0x49
    0, 0, 0, 0, 0, 0,  //0x4A - 0x4F
    KEY_DOWN,          //0x50    //keypad down arrow
    KEY_KP_3,          //0x51    //keypad page down
    KEY_KP_0,          //0x52    //keypad insert key
    KEY_KP_DECIMAL,    //0x53    //keypad delete key
    KEY_UNKNOWN,       //0x54
    KEY_UNKNOWN,       //0x55
    KEY_UNKNOWN,       //0x56
    KEY_F11,           //0x57
    KEY_F12            //0x58
};

static char _scancode = 0;

static int _shift = 0;
static int _ctrl = 0;
static int _alt = 0;
static int _capslock = 0;
static int _numlock = 0;
static int _scrolllock = 0;

static char keyboard_keys[512];

static ListenerList_t key_pressed_listeners = { 0 };

static ListenerList_t key_released_listeners = { 0 };

//===========================
//   Keyboard Interface
//===========================

void keyboard_interrupt_handler();

uint8_t keyboard_ctrl_read_status();

uint8_t keyboard_enc_read_buffer();

void keyboard_ctrl_send_command(uint8_t command);

void keyboard_enc_send_command(uint8_t command);

void keyboard_set_LEDs(int num, int caps, int scroll);

uint8_t key_to_ASCI(int code);

//----------------------------
//   Initialize
//----------------------------
void Keyboard_init()
{
    // install interrupt handler
    IDT_install_int(PIC_INT_KEYBOARD, (uintptr_t)keyboard_interrupt_handler, 0x8, IDT_DESC_32_BIT | IDT_DESC_SEG_PRESENT);

    memset(&key_pressed_listeners, 0, sizeof(ListenerList_t));
    memset(&key_released_listeners, 0, sizeof(ListenerList_t));
}

//-------------------------------
//   Listeners List
//-------------------------------

void ListenerList_add(ListenerList_t *list, uintptr_t handle)
{
    list->handelers[list->counter] = handle;
    list->counter += sizeof(uintptr_t);
}

void AddKeyPressedListener(KeyboardCallBack function)
{
    ListenerList_add(&key_pressed_listeners, (uintptr_t)function);
}

void AddKeyReleasedListener(KeyboardCallBack function)
{
    ListenerList_add(&key_released_listeners, (uintptr_t)function);
}

int isKeyDown(uint32_t key)
{
    if (key > 512) { return 0; }
    return keyboard_keys[key];
}

//===========================
//   interrupt handler
//===========================

void keyboard_interrupt_handler()
{
    __asm__("pusha");
    __asm__("cli");  // disable hardware interrupts

    int code = 0;
    static int is_extended = 0;

    // read scan code only if the keyboard controller output buffer is full (scan code is in it)
    if (keyboard_ctrl_read_status() & KYBRD_CTRL_STATS_MASK_OUT_BUF)
    {
        code = keyboard_enc_read_buffer();
        int key = keyboard_scancodes[code];

        // is this an extended code ?
        if (code == 0xE0 || code == 0xE1)
        {
            is_extended = 1;
        }
        else
        {
            is_extended = 0;

            if (code & 0x80) // test 8th bit, 'Break' code, key released
            {
                code &= ~(0x80); // convert to 'Make' code

                key = keyboard_scancodes[code];

                // test if a special key has been released
                if (key == KEY_LCTRL || key == KEY_RCTRL)
                {
                    _ctrl = 0;
                }
                else if (key == KEY_LALT || key == KEY_RALT)
                {
                    _alt = 0;
                }
                else if (key == KEY_LSHIFT || key == KEY_RSHIFT)
                {
                    _shift = 0;
                }

                keyboard_keys[key] = 0;

                for (int i = 0 ; i < key_released_listeners.counter ; i++)
                {
                    KeyboardCallBack func = (KeyboardCallBack)key_released_listeners.handelers[i];
                    func(key);
                }
            }
            else // 'Make code', key pressed
            {
                _scancode = code;

                key = keyboard_scancodes[code];

                // test if a special key is held down
                if (key == KEY_LCTRL || key == KEY_RCTRL)
                {
                    _ctrl = 1;
                }
                else if (key == KEY_LALT || key == KEY_RALT)
                {
                    _alt = 1;
                }
                else if (key == KEY_LSHIFT || key == KEY_RSHIFT)
                {
                    _shift = 1;
                }
                else if (key == KEY_CAPSLOCK)
                {
                    _capslock = ~(_capslock);
                    keyboard_set_LEDs(_numlock, _capslock, _scrolllock);
                }
                else if (key == KEY_SCROLLLOCK)
                {
                    _scrolllock = ~(_scrolllock);
                    keyboard_set_LEDs(_numlock, _capslock, _scrolllock);
                }
                else if (key == KEY_KP_NUMLOCK)
                {
                    _numlock = ~(_numlock);
                    keyboard_set_LEDs(_numlock, _capslock, _scrolllock);
                }

                keyboard_keys[key] = 1;

                for (int i = 0 ; i < key_pressed_listeners.counter ; i++)
                {
                    KeyboardCallBack func = (KeyboardCallBack)key_pressed_listeners.handelers[i];
                    func(key);
                }

                char character[2] = {key_to_ASCI(key), 0};
                if (isASCI(key)) print(character);
            }
        }
    }

    Hardware_interrupt_done(PIC_IRQ_KEYBOARD);
    __asm__("sti");  // re-enable hardware interrupts
    __asm__("popa");
    __asm__("leave");
    __asm__("iret");
}

//=======================
//   utilities
//=======================

uint8_t key_to_ASCI(int code)
{
    uint8_t key = code;

    if (isASCI(key))
    {
        if (_shift || _capslock)
        {
            if (key >= 'a' && key <= 'z') { key -= 32; }
        }

        if (_shift && !_capslock)
        {
            if (key >= '0' && key <= '9')
            {
                switch (key)
                {
                    case '0':
                        key = KEY_RIGHTPARENTHESIS;
                        break;
                    case '1':
                        key = KEY_EXCLAMATION;
                        break;
                    case '2':
                        key = KEY_AT;
                        break;
                    case '3':
                        key = KEY_HASH;
                        break;
                    case '4':
                        key = KEY_DOLLAR;
                        break;
                    case '5':
                        key = KEY_PERCENT;
                        break;
                    case '6':
                        key = KEY_CARRET;
                        break;
                    case '7':
                        key = KEY_AMPERSAND;
                        break;
                    case '8':
                        key = KEY_ASTERISK;
                        break;
                    case '9':
                        key = KEY_LEFTPARENTHESIS;
                        break;
                }
            }
            else
            {
                switch (key)
                {
                    case KEY_COMMA:
                        key = KEY_LESS;
                        break;

                    case KEY_DOT:
                        key = KEY_GREATER;
                        break;

                    case KEY_SLASH:
                        key = KEY_QUESTION;
                        break;

                    case KEY_SEMICOLON:
                        key = KEY_COLON;
                        break;

                    case KEY_QUOTE:
                        key = KEY_QUOTEDOUBLE;
                        break;

                    case KEY_LEFTBRACKET :
                        key = KEY_LEFTCURL;
                        break;

                    case KEY_RIGHTBRACKET :
                        key = KEY_RIGHTCURL;
                        break;

                    case KEY_GRAVE:
                        key = KEY_TILDE;
                        break;

                    case KEY_MINUS:
                        key = KEY_UNDERSCORE;
                        break;

                    case KEY_PLUS:
                        key = KEY_EQUAL;
                        break;

                    case KEY_BACKSLASH:
                        key = KEY_BAR;
                        break;
                }
            }
        }

        // return the key
        return key;
    }

    return 0;
}

void keyboard_set_LEDs(int num, int caps, int scroll)
{
    uint8_t data = 0;

    // set or clear the bit
    data = (scroll) ? (data | 1) : (data & 1);
    data = (num) ? (data | 2) : (data & 2);
    data = (caps) ? (data | 4) : (data & 4);

    // send the command -- update keyboard Light Emetting Diods (LEDs)
    keyboard_enc_send_command(KYBRD_ENC_CMD_SET_LED);
    keyboard_enc_send_command(data);
}

inline uint8_t keyboard_ctrl_read_status()
{
    return port_byte_in(KYBRD_CTRL_STATUS_REG);
}

inline uint8_t keyboard_enc_read_buffer()
{
    return port_byte_in(KYBRD_ENC_INPUT_BUFFER);
}

inline void keyboard_ctrl_send_command(uint8_t command)
{
    // wait for the input buffer to be empty
    while(keyboard_ctrl_read_status() & KYBRD_CTRL_STATS_MASK_IN_BUF){}

    port_byte_out(KYBRD_CTRL_CMD_REG, command);
}

inline void keyboard_enc_send_command(uint8_t command)
{
    // wait for the input buffer to be empty
    while(keyboard_ctrl_read_status() & KYBRD_CTRL_STATS_MASK_IN_BUF){}

    port_byte_out(KYBRD_ENC_CMD_REG, command);
}
