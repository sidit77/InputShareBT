#pragma once

#include <windows.h>
#include <functional>
#include "keys.h"

struct KeyEventArgs {
    VirtualKey key;
    uint16_t scanCode;
    KeyState pressed;
    KeyEventArgs(VirtualKey key, uint16_t scanCode, KeyState pressed) : key(key), scanCode(scanCode), pressed(pressed) {}
};

typedef std::function<bool(KeyEventArgs)> KeyCallback;

class InputHook {
public:
    InputHook() = delete;
    static void Initialize(KeyCallback keyCallback);
};

