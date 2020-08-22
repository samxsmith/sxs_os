#include "keyboard.h"
#include "../kernel/ports.h"
#include "../kernel/interrupt_service_routines.h"
#include "screen.h"
#include "../lib/libc/string.h"

char *get_keyboard_key(byte);

static char key_buffer[256];

const char *scan_code_names[] = {
    "ERROR", "Esc", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
    "-", "=", "Backspace", "Tab", "Q", "W", "E", "R", "T", "Y", "U",
    "I", "O", "P", "[", "]", "Enter", "Lctrl", "A", "S", "D", "F", "G",
    "H", "J", "K", "L", ";", "'", "`", "LShift", "\\", "Z", "X", "C",
    "V", "B", "N", "M", ",", ".", "/", "RShift", "Keypad *", "LAlt", "Spacebar"
};

const char scan_code_acsii[] = {
    '?', '?', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
    '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']',
    '?', '?', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`',
    '?', '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '
};

#define SCAN_MODE_MAX 57
#define BACKSPACE 0x0E
#define ENTER 0x1C

static void keyboard_callback(Register_Table regs) {
    // the PIC writes the scancode for us to read at port 0x60
    byte scancode = read_byte_from_port(0x60);
    if (scancode < SCAN_MODE_MAX) return;
    if (scancode == BACKSPACE) {
        backspace(key_buffer); // <-----------?
        screen_print_backspace();
    } else if (scancode == ENTER) {
        screen_print("\n");
        user_input(key_buffer); // <-------------?
        key_buffer[0] = '\0';
    } else {
        char letter = scan_code_acsii[(int)scancode];
        append(key_buffer, letter);
        screen_print_char(letter);
    }

    UNUSED(regs); // <------------------?
}

void init_keyboard() {
	register_interrupt_handler(IRQ1, keyboard_callback);
}

// static void keyboard_callback(Register_Table rt) {
// 	// the PIC writes the scancode for us to read at port 0x60
// 	byte scancode = read_byte_from_port(0x60);
// 	char *scancode_ascii;
// 	itoa(scancode, scancode_ascii);
// 	screen_print("Keyboard scancode: ");
// 	screen_print(scancode_ascii);
// 	screen_print(", ");
// 	char *key = get_keyboard_key(scancode);
//     screen_print(key);
// 	screen_print("\n");
// }


// char *get_keyboard_key(byte scancode) {
//     switch (scancode) {
//         case 0x0:
//             return "ERROR";
//         case 0x1:
//             return "ESC";
//         case 0x2:
//             return "1";
//         case 0x3:
//             return "2";
//         case 0x4:
//             return "3";
//         case 0x5:
//             return "4";
//         case 0x6:
//             return "5";
//         case 0x7:
//             return "6";
//         case 0x8:
//             return "7";
//         case 0x9:
//             return "8";
//         case 0x0A:
//             return "9";
//         case 0x0B:
//             return "0";
//         case 0x0C:
//             return "-";
//         case 0x0D:
//             return "+";
//         case 0x0E:
//             return "Backspace";
//         case 0x0F:
//             return "Tab";
//         case 0x10:
//             return "Q";
//         case 0x11:
//             return "W";
//         case 0x12:
//             return "E";
//         case 0x13:
//             return "R";
//         case 0x14:
//             return "T";
//         case 0x15:
//             return "Y";
//         case 0x16:
//             return "U";
//         case 0x17:
//             return "I";
//         case 0x18:
//             return "O";
//         case 0x19:
//             return "P";
// 		case 0x1A:
// 			return "[";
// 		case 0x1B:
// 			return "]";
// 		case 0x1C:
// 			return "ENTER";
// 		case 0x1D:
// 			return "LCtrl";
// 		case 0x1E:
// 			return "A";
// 		case 0x1F:
// 			return "S";
//         case 0x20:
//             return "D";
//         case 0x21:
//             return "F";
//         case 0x22:
//             return "G";
//         case 0x23:
//             return "H";
//         case 0x24:
//             return "J";
//         case 0x25:
//             return "K";
//         case 0x26:
//             return "L";
//         case 0x27:
//             return ";";
//         case 0x28:
//             return "'";
//         case 0x29:
//             return "`";
// 		case 0x2A:
// 			return "LShift";
// 		case 0x2B:
// 			return "\\";
// 		case 0x2C:
// 			return "Z";
// 		case 0x2D:
// 			return "X";
// 		case 0x2E:
// 			return "C";
// 		case 0x2F:
// 			return "V";
//         case 0x30:
//             return "B";
//         case 0x31:
//             return "N";
//         case 0x32:
//             return "M";
//         case 0x33:
//             return ",";
//         case 0x34:
//             return ".";
//         case 0x35:
//             return "/";
//         case 0x36:
//             return "Rshift";
//         case 0x37:
//             return "Keypad *";
//         case 0x38:
//             return "LAlt";
//         case 0x39:
//             return "Spc";
//         default:
// 			if (scancode <= 0x7f) {
// 				screen_print("Unknown key down");
// 				return "";
// 			}

// 			// more info on scancodes: https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
// 			// here we've implemented simple US scancodes
// 			// the 'KeyUp' interrupt fired by PIT gives the code "key pressed + 0x80"
// 			// e.g. KeyDown "." is code 0x34
// 			// so KeyUp "." is code 0x34 + 0x80 = 0xB4
// 			// 0x39 is the max keydown code we support
// 			// so if scancode is less than 0x39 + 0x80 = 0xB9
// 			// then we know it's a keyup code
// 			if (scancode <= 0xB9) {
// 				screen_print("Key up: ");
// 				return get_keyboard_key(scancode - 0x80);
// 			}

// 			screen_print("Unknown key up");
// 			return "";
//     }
// }

