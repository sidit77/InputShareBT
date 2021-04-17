#pragma once

#include <array>
#include "keys.h"
#include "hid.h"

typedef std::array<HIDKeyCode, 6> HIDKeys;

void bt_init();

void bt_send_char(HIDModifierKeys mod,  HIDKeys key);

void bt_destroy();
