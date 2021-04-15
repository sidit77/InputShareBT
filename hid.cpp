
#include <array>
#include "hid.h"

namespace {

    template<typename E>
    constexpr auto to_integral(E e) -> typename std::underlying_type<E>::type
    {
        return static_cast<typename std::underlying_type<E>::type>(e);
    }

    constexpr auto vk2hid = []{
        std::array<uint8_t, 255> table = {};

        table[to_integral(VirtualKey::KeyA)] = 0x04; // Keyboard a and A
        table[to_integral(VirtualKey::KeyB)] = 0x05; // Keyboard b and B
        table[to_integral(VirtualKey::KeyC)] = 0x06; // Keyboard c and C
        table[to_integral(VirtualKey::KeyD)] = 0x07; // Keyboard d and D
        table[to_integral(VirtualKey::KeyE)] = 0x08; // Keyboard e and E
        table[to_integral(VirtualKey::KeyF)] = 0x09; // Keyboard f and F
        table[to_integral(VirtualKey::KeyG)] = 0x0A; // Keyboard g and G
        table[to_integral(VirtualKey::KeyH)] = 0x0B; // Keyboard h and H
        table[to_integral(VirtualKey::KeyI)] = 0x0C; // Keyboard i and I
        table[to_integral(VirtualKey::KeyJ)] = 0x0D; // Keyboard j and J
        table[to_integral(VirtualKey::KeyK)] = 0x0E; // Keyboard k and K
        table[to_integral(VirtualKey::KeyL)] = 0x0F; // Keyboard l and L
        table[to_integral(VirtualKey::KeyM)] = 0x10; // Keyboard m and M
        table[to_integral(VirtualKey::KeyN)] = 0x11; // Keyboard n and N
        table[to_integral(VirtualKey::KeyO)] = 0x12; // Keyboard o and O
        table[to_integral(VirtualKey::KeyP)] = 0x13; // Keyboard p and P
        table[to_integral(VirtualKey::KeyQ)] = 0x14; // Keyboard q and Q
        table[to_integral(VirtualKey::KeyR)] = 0x15; // Keyboard r and R
        table[to_integral(VirtualKey::KeyS)] = 0x16; // Keyboard s and S
        table[to_integral(VirtualKey::KeyT)] = 0x17; // Keyboard t and T
        table[to_integral(VirtualKey::KeyU)] = 0x18; // Keyboard u and U
        table[to_integral(VirtualKey::KeyV)] = 0x19; // Keyboard v and V
        table[to_integral(VirtualKey::KeyW)] = 0x1A; // Keyboard w and W
        table[to_integral(VirtualKey::KeyX)] = 0x1B; // Keyboard x and X
        table[to_integral(VirtualKey::KeyY)] = 0x1C; // Keyboard y and Y
        table[to_integral(VirtualKey::KeyZ)] = 0x1D; // Keyboard z and Z

table[to_integral(VirtualKey::Key1)] = 0x1E;  // Keyboard 1 and !
table[to_integral(VirtualKey::Key2)] = 0x1F;  // Keyboard 2 and @
table[to_integral(VirtualKey::Key3)] = 0x20;  // Keyboard 3 and #
table[to_integral(VirtualKey::Key4)] = 0x21;  // Keyboard 4 and $
table[to_integral(VirtualKey::Key5)] = 0x22;  // Keyboard 5 and %
table[to_integral(VirtualKey::Key6)] = 0x23;  // Keyboard 6 and ^
table[to_integral(VirtualKey::Key7)] = 0x24;  // Keyboard 7 and &
table[to_integral(VirtualKey::Key8)] = 0x25;  // Keyboard 8 and *
table[to_integral(VirtualKey::Key9)] = 0x26;  // Keyboard 9 and (
table[to_integral(VirtualKey::Key0)] = 0x27;  // Keyboard 0 and )

//table[to_integral(VirtualKey::Return)] = 0x28; // Keyboard Return (ENTER)
//table[to_integral(VirtualKey::Escape)] = 0x29; // Keyboard ESCAPE
//table[to_integral(VirtualKey::Delete)] = 0x2A; // Keyboard DELETE (Backspace)
//table[to_integral(VirtualKey::Tab)] = 0x2B; // Keyboard Tab
//table[to_integral(VirtualKey::Space)] = 0x2C; // Keyboard Spacebar
//table[to_integral(VirtualKey::OemMinus)] = 0x2D; // Keyboard - and _
//table[to_integral(VirtualKey::Eq     )] = 0x2E; // Keyboard = and +
//table[to_integral(VirtualKey::Leftbrace )] = 0x2F; // Keyboard [ and {
//table[to_integral(VirtualKey::Rightbrace)] = 0x30; // Keyboard ] and }
//table[to_integral(VirtualKey::Backslash )] = 0x31; // Keyboard \ and |
//table[to_integral(VirtualKey::Hashtilde )] = 0x32; // Keyboard Non-US # and ~
//table[to_integral(VirtualKey::Semicolon )] = 0x33; // Keyboard ; and :
//table[to_integral(VirtualKey::Apostrophe)] = 0x34; // Keyboard ' and "
//table[to_integral(VirtualKey::Grave     )] = 0x35; // Keyboard ` and ~
//table[to_integral(VirtualKey::Comma     )] = 0x36; // Keyboard , and <
//table[to_integral(VirtualKey::Dot       )] = 0x37; // Keyboard . and >
//table[to_integral(VirtualKey::Slash     )] = 0x38; // Keyboard / and ?
//table[to_integral(VirtualKey::Capslock  )] = 0x39; // Keyboard Caps Lock

table[to_integral(VirtualKey::F1)]  = 0x3A; // Keyboard F1
table[to_integral(VirtualKey::F2)]  = 0x3B; // Keyboard F2
table[to_integral(VirtualKey::F3)]  = 0x3C; // Keyboard F3
table[to_integral(VirtualKey::F4)]  = 0x3D; // Keyboard F4
table[to_integral(VirtualKey::F5)]  = 0x3E; // Keyboard F5
table[to_integral(VirtualKey::F6)]  = 0x3F; // Keyboard F6
table[to_integral(VirtualKey::F7)]  = 0x40; // Keyboard F7
table[to_integral(VirtualKey::F8)]  = 0x41; // Keyboard F8
table[to_integral(VirtualKey::F9)]  = 0x42; // Keyboard F9
table[to_integral(VirtualKey::F10)] = 0x43; // Keyboard F10
table[to_integral(VirtualKey::F11)] = 0x44; // Keyboard F11
table[to_integral(VirtualKey::F12)] = 0x45; // Keyboard F12

        return table;
    }();
}

uint8_t getHidKeycode(VirtualKey key) {
    return vk2hid[to_integral(key)];
}


const uint8_t KEYCODE_WINDOWS_TO_HID[256] = {
        0,41,30,31,32,33,34,35,36,37,38,39,45,46,42,43,20,26,8,21,23,28,24,12,18,19,
        47,48,158,224,4,22,7,9,10,11,13,14,15,51,52,53,225,49,29,27,6,25,5,17,16,54,
        55,56,229,0,226,0,57,58,59,60,61,62,63,64,65,66,67,72,71,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,68,69,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,228,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,70,230,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,74,82,75,0,80,0,79,0,77,81,78,73,76,0,0,0,0,0,0,0,227,231,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

uint8_t keycode_windows_to_hid(unsigned scancode) {
    if (scancode >= 256)
        return 0;
    return KEYCODE_WINDOWS_TO_HID[scancode];
}

static const char KEYCODE_WINDOWS_NAME_DATA[] =
        "\0'\0,\0-\0.\0/\0;\0=\0A\0B\0Backspace\0C\0Caps Lock\0D\0Delete\0E\0End\0"
        "Enter\0Escape\0F\0F1\0F10\0F11\0F12\0F2\0F3\0F4\0F5\0F6\0F7\0F8\0F9\0G\0H"
        "\0Home\0I\0Insert\0J\0K\0L\0Left\0Left Alt\0Left Control\0Left Shift\0Lef"
        "t Windows\0M\0N\0O\0P\0Page Down\0Page Up\0Pause\0Print Screen\0Q\0R\0Rig"
        "ht\0Right Alt\0Right Control\0Right Shift\0Right Windows\0S\0Scroll Lock"
        "\0T\0Tab\0U\0V\0W\0X\0Y\0Z\0[\0\\\0]\0`";
static const unsigned short KEYCODE_WINDOWS_NAME_OFFSET[] = {
        0,0,0,0,15,17,29,41,50,69,110,112,119,128,130,132,185,187,189,191,230,232,
        290,304,310,312,314,316,318,320,72,84,90,93,96,99,102,105,108,76,0,62,19,
        306,0,5,13,322,326,324,0,11,1,328,3,7,9,31,71,86,89,92,95,98,101,104,107,74,
        78,82,217,292,211,121,114,203,43,52,193,234,134,198,208,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,56,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,148,161,139,172,250,264,240,276
};
const char *keycode_windows_name(unsigned index) {
    unsigned offset;
    if (232 <= index)
        return 0;
    offset = KEYCODE_WINDOWS_NAME_OFFSET[index];
    if (offset == 0)
        return 0;
    return KEYCODE_WINDOWS_NAME_DATA + offset;
}
