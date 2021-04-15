#pragma once

#include <cstdint>
#include "keys.h"

uint8_t getHidKeycode(VirtualKey key);


uint8_t keycode_windows_to_hid(unsigned scancode);


const char *keycode_windows_name(unsigned index);