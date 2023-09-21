#ifndef keyboard_included
#define keyboard_included

#include <libc/stdint.h>

//==============================
//   Keyboard Interface
//==============================

extern void Keyboard_init();

extern int isKeyDown(uint32_t key);

typedef void (*KeyboardCallBack)(int);

//-----------------------------
//   listeners list
//----------------------------
typedef struct _ListenerList_t
{
    uintptr_t handelers[64];
    uint32_t counter;
} ListenerList_t;

extern void ListenerList_add(ListenerList_t *list, uintptr_t handle);

extern void AddKeyboardListener(KeyboardCallBack function);

//==============================
//   Key Codes
//==============================

#define isASCI(x) ((x) <= 0x7F)

typedef enum _KEYCODE
{
// Alphanumeric keys ////////////////

    KEY_SPACE             = ' ',
    KEY_0                 = '0',
    KEY_1                 = '1',
    KEY_2                 = '2',
    KEY_3                 = '3',
    KEY_4                 = '4',
    KEY_5                 = '5',
    KEY_6                 = '6',
    KEY_7                 = '7',
    KEY_8                 = '8',
    KEY_9                 = '9',

    KEY_A                 = 'a',
    KEY_B                 = 'b',
    KEY_C                 = 'c',
    KEY_D                 = 'd',
    KEY_E                 = 'e',
    KEY_F                 = 'f',
    KEY_G                 = 'g',
    KEY_H                 = 'h',
    KEY_I                 = 'i',
    KEY_J                 = 'j',
    KEY_K                 = 'k',
    KEY_L                 = 'l',
    KEY_M                 = 'm',
    KEY_N                 = 'n',
    KEY_O                 = 'o',
    KEY_P                 = 'p',
    KEY_Q                 = 'q',
    KEY_R                 = 'r',
    KEY_S                 = 's',
    KEY_T                 = 't',
    KEY_U                 = 'u',
    KEY_V                 = 'v',
    KEY_W                 = 'w',
    KEY_X                 = 'x',
    KEY_Y                 = 'y',
    KEY_Z                 = 'z',

    KEY_RETURN            = '\r',
    KEY_ESCAPE            = 0x101,
    KEY_BACKSPACE         = '\b',

// Arrow keys ////////////////////////

    KEY_UP                = 0x102,
    KEY_DOWN              = 0x103,
    KEY_LEFT              = 0x104,
    KEY_RIGHT             = 0x105,

// Function keys /////////////////////

    KEY_F1                = 0x106,
    KEY_F2                = 0x107,
    KEY_F3                = 0x108,
    KEY_F4                = 0x109,
    KEY_F5                = 0x10A,
    KEY_F6                = 0x10B,
    KEY_F7                = 0x10C,
    KEY_F8                = 0x10D,
    KEY_F9                = 0x10E,
    KEY_F10               = 0x10F,
    KEY_F11               = 0x110,
    KEY_F12               = 0x111,
    KEY_F13               = 0x112,
    KEY_F14               = 0x113,
    KEY_F15               = 0x114,

    KEY_DOT               = '.',
    KEY_COMMA             = ',',
    KEY_COLON             = ':',
    KEY_SEMICOLON         = ';',
    KEY_SLASH             = '/',
    KEY_BACKSLASH         = '\\',
    KEY_PLUS              = '+',
    KEY_MINUS             = '-',
    KEY_ASTERISK          = '*',
    KEY_EXCLAMATION       = '!',
    KEY_QUESTION          = '?',
    KEY_QUOTEDOUBLE       = '\"',
    KEY_QUOTE             = '\'',
    KEY_EQUAL             = '=',
    KEY_HASH              = '#',
    KEY_PERCENT           = '%',
    KEY_AMPERSAND         = '&',
    KEY_UNDERSCORE        = '_',
    KEY_LEFTPARENTHESIS   = '(',
    KEY_RIGHTPARENTHESIS  = ')',
    KEY_LEFTBRACKET       = '[',
    KEY_RIGHTBRACKET      = ']',
    KEY_LEFTCURL          = '{',
    KEY_RIGHTCURL         = '}',
    KEY_DOLLAR            = '$',
    KEY_POUND             = '$',
    KEY_EURO              = '$',
    KEY_LESS              = '<',
    KEY_GREATER           = '>',
    KEY_BAR               = '|',
    KEY_GRAVE             = '`',
    KEY_TILDE             = '~',
    KEY_AT                = '@',
    KEY_CARRET            = '^',

// Numeric keypad //////////////////////

    KEY_KP_0              = '0',
    KEY_KP_1              = '1',
    KEY_KP_2              = '2',
    KEY_KP_3              = '3',
    KEY_KP_4              = '4',
    KEY_KP_5              = '5',
    KEY_KP_6              = '6',
    KEY_KP_7              = '7',
    KEY_KP_8              = '8',
    KEY_KP_9              = '9',
    KEY_KP_PLUS           = '+',
    KEY_KP_MINUS          = '-',
    KEY_KP_DECIMAL        = '.',
    KEY_KP_DIVIDE         = '/',
    KEY_KP_ASTERISK       = '*',
    KEY_KP_NUMLOCK        = 0x115,
    KEY_KP_ENTER          = 0x116,

    KEY_TAB               = 0x117,
    KEY_CAPSLOCK          = 0x118,

// Modify keys ////////////////////////////

    KEY_LSHIFT            = 0x119,
    KEY_LCTRL             = 0x11A,
    KEY_LALT              = 0x11B,
    KEY_LWIN              = 0x11C,
    KEY_RSHIFT            = 0x11D,
    KEY_RCTRL             = 0x11E,
    KEY_RALT              = 0x11F,
    KEY_RWIN              = 0x120,

    KEY_INSERT            = 0x121,
    KEY_DELETE            = 0x122,
    KEY_HOME              = 0x123,
    KEY_END               = 0x124,
    KEY_PAGEUP            = 0x125,
    KEY_PAGEDOWN          = 0x126,
    KEY_SCROLLLOCK        = 0x127,
    KEY_PAUSE             = 0x128,

    KEY_UNKNOWN           = 0x129,
    KEY_NUMKEYCODES       = 0x12A
} KEYCODE;

#endif // keyboard_included
