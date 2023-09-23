
#include <kernel/user/cmd.h>
#include <kernel/input/keyboard.h>
#include <kernel/graphics/VGA.h>
#include <libc/stdint.h>
#include <libc/string.h>

//=====================================
//   Command Line Interface
//=====================================

typedef struct _TextBuffer_t
{
    char buffer[512];
    size_t count;
} TextBuffer_t;

TextBuffer_t cmd_buffer;

int cmd_current_line = 0;

char *root_dir_txt = "Command>";

char *os_logo_txt ="\
================\n\
     AMK-OS     \n\
================\n\
";

char *help_msg ="\
\n\nAvailable Commands: \n\
  -cls : clears the screen.\n\
  -help : prints this message\n.\
";

//--------------------------------
//   forward declarations
//--------------------------------
void keyboard_listener(int key);

void UpdateLineEnd();

void ParseCommand(char* text);

//----------------------------
//   Initialize
//----------------------------
void CMD_start()
{
    AddKeyPressedListener((uintptr_t)keyboard_listener);

    clear_screen();
    print(os_logo_txt);
    UpdateLineEnd();
}

void TextBuffer_Append(TextBuffer_t *buffer, char asci)
{
    buffer->buffer[buffer->count] = asci;
    buffer->count++;
}

void UpdateLineEnd()
{
    print("\n");
    print(root_dir_txt);
    cmd_current_line = VGA_current_mem;
}

void ParseCommand(char* text)
{
    if (strlen(text) > 0)
    {
        if (strcmp(text, "help"))
        {
            print(help_msg);
        }
        else if (strcmp(text, "cls"))
        {
            clear_screen();
            print(os_logo_txt);
        }
        else
        {
            print("\nUnknown Command.");
        }
    }

    // end of command
    UpdateLineEnd();
}

void keyboard_listener(int key)
{
    char asci[2] = {key_to_ASCI(key), 0};

    if(key == KEY_KP_ENTER)
    {
        ParseCommand(cmd_buffer.buffer);
        memset(&cmd_buffer, 0, sizeof(TextBuffer_t));
    }
    else if (key == KEY_BACKSPACE)
    {
        // reached end of current line
        if (cmd_current_line >= VGA_current_mem)
        {
            return;
        }

        uint8_t* mem = (uint8_t*)VIDEO_MEMORY;

        if (VGA_current_mem <= 0) { VGA_current_mem = 0; }

        VGA_current_mem -= 2;
        mem[VGA_current_mem] = 0;
        
        VGA_update_position(VGA_current_mem);
    }
    else if (isASCI(key))
    {
        TextBuffer_Append(&cmd_buffer, asci[0]);
        print(asci);
    }
}
